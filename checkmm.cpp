// Metamath database verifier
// Eric Schmidt (erics41293@gmail.com)
//
// I release this code to the public domain under the
// Creative Commons "CC0 1.0 Universal" Public Domain Dedication:
//
// http://creativecommons.org/publicdomain/zero/1.0/
//
// This is a standalone verifier for Metamath database files,
// written in portable C++. Run it with a single file name as the
// parameter.
//
// Some notes:
//
// The code assumes that the character set is compatible with ASCII.
//
// According to the spec, file inclusion commands should not include a file
// that has already been included. Unfortunately, determing whether two
// different strings refer to the same file is not easy, and, worse, is
// system-dependant. This program ignores the issue entirely and assumes
// that distinct strings name different files. This should be adequate for
// the present, at least.
//
// If the verifier finds an error, it will report it and quit. It will not
// attempt to recover and find more errors. The only condition that generates
// a diagnostic message but doesn't halt the program is an incomplete proof,
// specified by a question mark. In that case, as per the spec, a warning is
// issued and checking continues.
//
// Please let me know of any bugs.

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace
{

std::queue<std::string> tokens;

std::unordered_set<std::string> constants;

typedef std::vector<std::string> Expression;

// The first parameter is the statement of the hypothesis, the second is
// true iff the hypothesis is floating.
typedef std::pair<Expression, bool> Hypothesis;

std::unordered_map<std::string, Hypothesis> hypotheses;

std::unordered_set<std::string> variables;

// An axiom or a theorem.
struct Assertion
{
    // Hypotheses of this axiom or theorem.
    std::deque<std::string> hypotheses;
    std::set<std::pair<std::string, std::string> > disjvars;
    // Statement of axiom or theorem.
    Expression expression;
};

std::unordered_map<std::string, Assertion> assertions;

struct Scope
{
    std::unordered_set<std::string> activevariables;
    // Labels of active hypotheses
    std::vector<std::string> activehyp;
    std::vector<std::set<std::string> > disjvars;
    // Map from variable to label of active floating hypothesis
    std::unordered_map<std::string, std::string> floatinghyp;
};

std::vector<Scope> scopes;

// Determine if a string is used as a label
inline bool labelused(std::string const label)
{
    return hypotheses.find(label) != hypotheses.end()
        || assertions.find(label) != assertions.end();
}

// Find active floating hypothesis corresponding to variable, or empty string
// if there isn't one.
std::string getfloatinghyp(std::string const var)
{
    for (std::vector<Scope>::const_iterator iter(scopes.begin());
         iter != scopes.end(); ++iter)
    {
        std::unordered_map<std::string, std::string>::const_iterator const loc
            (iter->floatinghyp.find(var));
        if (loc != iter->floatinghyp.end())
            return loc->second;
    }

    return std::string();
}

// Determine if a string is an active variable.
bool isactivevariable(std::string const str)
{
    for (std::vector<Scope>::const_iterator iter(scopes.begin());
         iter != scopes.end(); ++iter)
    {
        if (iter->activevariables.find(str) != iter->activevariables.end())
            return true;
    }
    return false;
}

// Determine if a string is the label of an active hypothesis.
bool isactivehyp(std::string const str)
{
    for (std::vector<Scope>::const_iterator iter(scopes.begin());
         iter != scopes.end(); ++iter)
    {
        if (std::find(iter->activehyp.begin(), iter->activehyp.end(), str)
            != iter->activehyp.end())
            return true;
    }
    return false;
}

// Determine if there is an active disjoint variable restriction on
// two different variables.
bool isdvr(std::string var1, std::string var2)
{
    if (var1 == var2)
        return false;
    for (std::vector<Scope>::const_iterator iter(scopes.begin());
         iter != scopes.end(); ++iter)
    {
        for (std::vector<std::set<std::string> >::const_iterator iter2
            (iter->disjvars.begin()); iter2 != iter->disjvars.end(); ++iter2)
        {
            if (   iter2->find(var1) != iter2->end()
                && iter2->find(var2) != iter2->end())
                return true;
        }
    }
    return false;
}

// Determine if a character is white space in Metamath.
inline bool ismmws(char const ch)
{
    // This doesn't include \v ("vertical tab"), as the spec omits it.
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\f' || ch == '\r';
}

// Determine if a token is a label token.
bool islabeltoken(std::string const token)
{
    for (std::string::const_iterator iter(token.begin()); iter != token.end();
         ++iter)
    {
        unsigned char const ch(*iter);
        if (!(std::isalnum(ch) || ch == '.' || ch == '-' || ch == '_'))
            return false;
    }
    return true;
}

// Determine if a token is a math symbol token.
inline bool ismathsymboltoken(std::string const token)
{
    return token.find('$') == std::string::npos;
}

// Determine if a token consists solely of upper-case letters or question marks
bool containsonlyupperorq(std::string token)
{
    for (std::string::const_iterator iter(token.begin()); iter != token.end();
         ++iter)
    {
        if (!std::isupper(*iter) && *iter != '?')
            return false;
    }
    return true;
}

std::string nexttoken(std::istream & input)
{
    char ch;
    std::string token;

    // Skip whitespace
    while (input.get(ch) && ismmws(ch)) { }
    if (input.good())
        input.unget();

    // Get token
    while (input.get(ch) && !ismmws(ch))
    {
        if (ch < '!' || ch > '~')
        {
            std::cerr << "Invalid character read with code 0x";
            std::cerr << std::hex << (unsigned int)(unsigned char)ch
                      << std::endl;
            return std::string();
        }

        token += ch;
    }

    if (!input.eof() && input.fail())
        return std::string();

    return token;
}

bool readtokens(std::string const filename)
{
    static std::set<std::string> names;

    bool const alreadyencountered(!names.insert(filename).second);
    if (alreadyencountered)
        return true;

    std::ifstream in(filename.c_str());
    if (!in)
    {
        std::cerr << "Could not open " << filename << std::endl;
        return false;
    }

    bool incomment(false);
    bool infileinclusion(false);
    std::string newfilename;

    std::string token;
    while (!(token = nexttoken(in)).empty())
    {
        if (incomment)
        {
            if (token == "$)")
            {
                incomment = false;
                continue;
            }
            if (token.find("$(") != std::string::npos)
            {
                std::cerr << "Characters $( found in a comment" << std::endl;
                return false;
            }
            if (token.find("$)") != std::string::npos)
            {
                std::cerr << "Characters $) found in a comment" << std::endl;
                return false;
            }
            continue;
        }

        // Not in comment
        if (token == "$(")
        {
            incomment = true;
            continue;
        }

        if (infileinclusion)
        {
            if (newfilename.empty())
            {
                if (token.find('$') != std::string::npos)
                {
                    std::cerr << "Filename " << token << " contains a $"
                              << std::endl;
                    return false;
                }
                newfilename = token;
                continue;
            }
            else
            {
                if (token != "$]")
                {
                    std::cerr << "Didn't find closing file inclusion delimiter"
                              << std::endl;
                    return false;
                }

                bool const okay(readtokens(newfilename));
                if (!okay)
                    return false;
                infileinclusion = false;
                newfilename.clear();

                // Leave the $] token in the vector of tokens.
                // This will allow us to enforce that included files
                // do not contain incomplete statements.
                tokens.push(token);

                continue;
            }
        }

        if (token == "$[")
        {
            infileinclusion = true;

            // Leave the $[ token in the vector of tokens.
            // This will allow us to enforce that file inclusion does
            // not occur in the middle of a command or in an inner scope.
            tokens.push(token);

            continue;
        }

        if (token == "$]")
        {
            std::cerr << "Stray closing file inclusion delimiter" << std::endl;
            return false;
        }

        tokens.push(token);
    }

    if (!in.eof())
    {
        if (in.fail())
            std::cerr << "Error reading from " << filename << std::endl;
        return false;
    }

    if (incomment)
    {
        std::cerr << "Unclosed comment" << std::endl;
        return false;
    }

    if (infileinclusion)
    {
        std::cerr << "Unfinished file inclusion command" << std::endl;
        return false;
    }

    return true;
}

// Construct an Assertion from an Expression. That is, determine the
// mandatory hypotheses and disjoint variable restrictions.
// The Assertion is inserted into the assertions collection,
// and is returned by reference.
Assertion & constructassertion(std::string const label, Expression const & exp)
{
    Assertion & assertion
        (assertions.insert(std::make_pair(label, Assertion())).first->second);

    assertion.expression = exp;

    std::set<std::string> varsused;

    // Determine variables used and find mandatory hypotheses

    for (Expression::const_iterator iter(exp.begin()); iter != exp.end();
         ++iter)
    {
        if (variables.find(*iter) != variables.end())
            varsused.insert(*iter);
    }

    for (std::vector<Scope>::const_reverse_iterator iter(scopes.rbegin());
         iter != scopes.rend(); ++iter)
    {
        std::vector<std::string> const & hypvec(iter->activehyp);
        for (std::vector<std::string>::const_reverse_iterator iter2
            (hypvec.rbegin()); iter2 != hypvec.rend(); ++iter2)
        {
            Hypothesis const & hyp(hypotheses.find(*iter2)->second);
            if (hyp.second && varsused.find(hyp.first[1]) != varsused.end())
            {
                // Mandatory floating hypothesis
                assertion.hypotheses.push_front(*iter2);
            }
            else if (!hyp.second)
            {
                // Essential hypothesis
                assertion.hypotheses.push_front(*iter2);
                for (Expression::const_iterator iter3(hyp.first.begin());
                     iter3 != hyp.first.end(); ++iter3)
                {
                    if (variables.find(*iter3) != variables.end())
                        varsused.insert(*iter3);
                }
            }
        }
    }

    // Determine mandatory disjoint variable restrictions
    for (std::vector<Scope>::const_iterator iter(scopes.begin());
         iter != scopes.end(); ++iter)
    {
        std::vector<std::set<std::string> > const & disjvars(iter->disjvars);
        for (std::vector<std::set<std::string> >::const_iterator iter2
            (disjvars.begin()); iter2 != disjvars.end(); ++iter2)
        {
            std::unordered_set<std::string> dset;
            std::set_intersection
                 (iter2->begin(), iter2->end(),
                  varsused.begin(), varsused.end(),
                  std::inserter(dset, dset.end()));

            for (std::unordered_set<std::string>::const_iterator diter(dset.begin());
                 diter != dset.end(); ++diter)
            {
                std::unordered_set<std::string>::const_iterator diter2(diter);
                ++diter2;
                for (; diter2 != dset.end(); ++diter2)
                    assertion.disjvars.insert(std::make_pair(*diter, *diter2));
            }
        }
    }

    return assertion;
}

enum class verify_result { Valid, Invalid, Incomplete };

// Read an expression from the token stream.
verify_result readexpression
    ( char stattype, std::string label, std::string terminator,
      Expression * exp)
{
    if (tokens.empty())
    {
        std::cerr << "Unfinished $" << stattype << " statement " << label
                  << std::endl;
        return verify_result::Invalid;
    }

    std::string type(tokens.front());

    if (constants.find(type) == constants.end())
    {
        std::cerr << "First symbol in $" << stattype << " statement " << label
                  << " is " << type << " which is not a constant" << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop();

    exp->push_back(type);

    std::string token;

    while (!tokens.empty() && (token = tokens.front()) != terminator)
    {
        tokens.pop();

        if (constants.find(token) == constants.end()
         && getfloatinghyp(token).empty())
        {
            std::cerr << "In $" << stattype << " statement " << label
                      << " token " << token
                      << " found which is not a constant or variable in an"
                         " active $f statement" << std::endl;
            return verify_result::Invalid;
        }

        exp->push_back(token);
    }

    if (tokens.empty())
    {
        std::cerr << "Unfinished $" << stattype << " statement " << label
                  << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop(); // Discard terminator token

    return verify_result::Valid;
}

// Make a substitution of variables. The result is put in "destination",
// which should be empty.
void makesubstitution
    (Expression const & original, std::unordered_map<std::string, Expression> const & substmap,
     Expression * destination
    )
{
    for (Expression::const_iterator iter(original.begin());
         iter != original.end(); ++iter)
    {
        std::unordered_map<std::string, Expression>::const_iterator const iter2
            (substmap.find(*iter));
        if (iter2 == substmap.end())
        {
            // Constant
            destination->push_back(*iter);
        }
        else
        {
            // Variable
            std::copy(iter2->second.begin(), iter2->second.end(),
                      std::back_inserter(*destination));
        }
    }
}

// Get the raw numbers from compressed proof format.
// The letter Z is translated as 0.
verify_result getproofnumbers(
    std::string label, std::string proof, std::vector<std::size_t> * proofnumbers)
{
    std::size_t const size_max(std::numeric_limits<std::size_t>::max());

    std::size_t num(0u);
    bool justgotnum(false);
    for (std::string::const_iterator iter(proof.begin()); iter != proof.end();
         ++iter)
    {
        if (*iter <= 'T')
        {
            std::size_t const addval(*iter - ('A' - 1));

            if (num > size_max / 20 || 20 * num > size_max - addval)
            {
                std::cerr << "Overflow computing numbers in compressed proof "
                             "of " << label << std::endl;
                return verify_result::Invalid;
            }

            proofnumbers->push_back(20 * num + addval);
            num = 0;
            justgotnum = true;
        }
        else if (*iter <= 'Y')
        {
            std::size_t const addval(*iter - 'T');

            if (num > size_max / 5 || 5 * num > size_max - addval)
            {
                std::cerr << "Overflow computing numbers in compressed proof "
                             "of " << label << std::endl;
                return verify_result::Invalid;
            }

            num = 5 * num + addval;
            justgotnum = false;
        }
        else // It must be Z
        {
            if (!justgotnum)
            {
                std::cerr << "Stray Z found in compressed proof of "
                          << label << std::endl;
                return verify_result::Invalid;
            }

            proofnumbers->push_back(0);
            justgotnum = false;
        }
    }

    if (num != 0)
    {
        std::cerr << "Compressed proof of theorem " << label
                  << " ends in unfinished number" << std::endl;
        return verify_result::Invalid;
    }

    return verify_result::Valid;
}

// Subroutine for proof verification. Verify a proof step referencing an
// assertion (i.e., not a hypothesis).
verify_result verifyassertionref(
    std::string thlabel, std::string reflabel, std::vector<Expression> * stack)
{
    Assertion const & assertion(assertions.find(reflabel)->second);
    if (stack->size() < assertion.hypotheses.size())
    {
        std::cerr << "In proof of theorem " << thlabel
                  << " not enough items found on stack" << std::endl;
        return verify_result::Invalid;
    }

    std::vector<Expression>::size_type const base
        (stack->size() - assertion.hypotheses.size());

    std::unordered_map<std::string, Expression> substitutions;

    // Determine substitutions and check that we can unify
    for (std::deque<std::string>::size_type i(0);
         i < assertion.hypotheses.size(); ++i)
    {
        Hypothesis const & hypothesis
            (hypotheses.find(assertion.hypotheses[i])->second);
        if (hypothesis.second)
        {
            // Floating hypothesis of the referenced assertion
            if (hypothesis.first[0] != (*stack)[base + i][0])
            {
                std::cout << "In proof of theorem " << thlabel
                          << " unification failed" << std::endl;
                return verify_result::Invalid;
            }
            Expression & subst(substitutions.insert
                (std::make_pair(hypothesis.first[1],
                 Expression())).first->second);
            Expression & substee = (*stack)[base + i];
            subst.insert(subst.end(), substee.begin() + 1, substee.end());
        }
        else
        {
            // Essential hypothesis
            Expression dest;
            makesubstitution(hypothesis.first, substitutions, &dest);
            if (dest != (*stack)[base + i])
            {
                std::cerr << "In proof of theorem "  << thlabel
                          << " unification failed" << std::endl;
                return verify_result::Invalid;
            }
        }
    }

    // Remove hypotheses from stack
    stack->erase(stack->begin() + base, stack->end());

    // Verify disjoint variable conditions
    for (std::set<std::pair<std::string, std::string> >::const_iterator
         iter(assertion.disjvars.begin());
         iter != assertion.disjvars.end(); ++iter)
    {
        Expression const & exp1(substitutions.find(iter->first)->second);
        Expression const & exp2(substitutions.find(iter->second)->second);

        std::unordered_set<std::string> exp1vars;
        for (Expression::const_iterator exp1iter(exp1.begin());
             exp1iter != exp1.end(); ++exp1iter)
        {
            if (variables.find(*exp1iter) != variables.end())
                exp1vars.insert(*exp1iter);
        }

        std::unordered_set<std::string> exp2vars;
        for (Expression::const_iterator exp2iter(exp2.begin());
             exp2iter != exp2.end(); ++exp2iter)
        {
            if (variables.find(*exp2iter) != variables.end())
                exp2vars.insert(*exp2iter);
        }

        for (std::unordered_set<std::string>::const_iterator exp1iter
            (exp1vars.begin()); exp1iter != exp1vars.end(); ++exp1iter)
        {
            for (std::unordered_set<std::string>::const_iterator exp2iter
                (exp2vars.begin()); exp2iter != exp2vars.end(); ++exp2iter)
            {
                if (!isdvr(*exp1iter, *exp2iter))
                {
                    std::cerr << "In proof of theorem " << thlabel
                              << " disjoint variable restriction violated"
                              << std::endl;
                    return verify_result::Invalid;
                }
            }
        }
    }

    // Done verification of this step. Insert new statement onto stack.
    Expression dest;
    makesubstitution(assertion.expression, substitutions, &dest);
    stack->push_back(dest);

    return verify_result::Valid;
}

// Verify a regular proof. The "proof" argument should be a non-empty sequence
// of valid labels.
verify_result verifyregularproof(
    std::string label, Assertion const & theorem,
    std::vector<std::string> const & proof)
{
    std::vector<Expression> stack;
    for (std::vector<std::string>::const_iterator proofstep(proof.begin());
         proofstep != proof.end(); ++proofstep)
    {
        // If step is a hypothesis, just push it onto the stack.
        std::unordered_map<std::string, Hypothesis>::const_iterator hyp
            (hypotheses.find(*proofstep));
        if (hyp != hypotheses.end())
        {
            stack.push_back(hyp->second.first);
            continue;
        }

        // It must be an axiom or theorem
        verify_result const result(verifyassertionref(label, *proofstep, &stack));
        if (result != verify_result::Valid)
            return result;
    }

    if (stack.size() != 1)
    {
        std::cerr << "Proof of theorem " << label
                  << " does not end with only one item on the stack"
                  << std::endl;
        return verify_result::Invalid;
    }

    if (stack[0] != theorem.expression)
    {
        std::cerr << "Proof of theorem " << label << " proves wrong statement"
                  << std::endl; 
    }

    return verify_result::Valid;
}

// Verify a compressed proof
verify_result verifycompressedproof
    (std::string label, Assertion const & theorem,
     std::vector<std::string> const & labels,
     std::vector<std::size_t> const & proofnumbers)
{
    std::vector<Expression> stack;

    std::size_t const mandhypt(theorem.hypotheses.size());
    std::size_t const labelt(mandhypt + labels.size());

    std::vector<Expression> savedsteps;
    for (std::vector<std::size_t>::const_iterator iter(proofnumbers.begin());
         iter != proofnumbers.end(); ++iter)
    {
        // Save the last proof step if 0
        if (*iter == 0)
        {
            savedsteps.push_back(stack.back());
            continue;
        }

        // If step is a mandatory hypothesis, just push it onto the stack.
        if (*iter <= mandhypt)
        {
            stack.push_back
                (hypotheses.find(theorem.hypotheses[*iter - 1])->second.first);
        }
        else if (*iter <= labelt)
        {
            std::string const proofstep(labels[*iter - mandhypt - 1]);

            // If step is a (non-mandatory) hypothesis,
            // just push it onto the stack.
            std::unordered_map<std::string, Hypothesis>::const_iterator hyp
            (hypotheses.find(proofstep));
            if (hyp != hypotheses.end())
            {
                stack.push_back(hyp->second.first);
                continue;
            }

            // It must be an axiom or theorem
            verify_result const result(verifyassertionref(label, proofstep, &stack));
            if (result != verify_result::Valid)
                return result;
        }
        else // Must refer to saved step
        {
            if (*iter > labelt + savedsteps.size())
            {
                std::cerr << "Number in compressed proof of " << label
                          << " is too high" << std::endl;
                return verify_result::Invalid;
            }

            stack.push_back(savedsteps[*iter - labelt - 1]);
        }
    }

    if (stack.size() != 1)
    {
        std::cerr << "Proof of theorem " << label
                  << " does not end with only one item on the stack"
                  << std::endl;
        return verify_result::Invalid;
    }

    if (stack[0] != theorem.expression)
    {
        std::cerr << "Proof of theorem " << label << " proves wrong statement"
                  << std::endl; 
    }

    return verify_result::Valid;
}

verify_result parsep(std::string label)
{
    Expression newtheorem;
    verify_result const result(readexpression('p', label, "$=", &newtheorem));
    if (result != verify_result::Valid)
    {
        return result;
    }

    Assertion const & assertion(constructassertion(label, newtheorem));

    // Now for the proof

    if (tokens.empty())
    {
        std::cerr << "Unfinished $p statement " << label << std::endl;
        return verify_result::Invalid;
    }

    if (tokens.front() == "(")
    {
        // Compressed proof
        tokens.pop();

        // Get labels

        std::vector<std::string> labels;
        std::string token;
        while (!tokens.empty() && (token = tokens.front()) != ")")
        {
            tokens.pop();
            labels.push_back(token);
            if (token == label)
            {
                std::cerr << "Proof of theorem " << label
                          << " refers to itself" << std::endl;
                return verify_result::Invalid;
            }
            else if (std::find
                    (assertion.hypotheses.begin(), assertion.hypotheses.end(),
                     token) != assertion.hypotheses.end())
            {
                std::cerr << "Compressed proof of theorem " << label
                          << " has mandatory hypothesis " << token
                          << " in label list" << std::endl;
                return verify_result::Invalid;
            }
            else if (assertions.find(token) == assertions.end()
                  && !isactivehyp(token))
            {
                std::cerr << "Proof of theorem " << label << " refers to "
                          << token << " which is not an active statement"
                          << std::endl;
                return verify_result::Invalid;
            }
        }

        if (tokens.empty())
        {
            std::cerr << "Unfinished $p statement " << label << std::endl;
            return verify_result::Invalid;
        }

        tokens.pop(); // Discard ) token

        // Get proof steps

        std::string proof;
        while (!tokens.empty() && (token = tokens.front()) != "$.")
        {
            tokens.pop();

            proof += token;
            if (!containsonlyupperorq(token))
            {
                std::cerr << "Bogus character found in compressed proof of "
                          << label << std::endl;
                return verify_result::Invalid;
            }
        }

        if (tokens.empty())
        {
            std::cerr << "Unfinished $p statement " << label << std::endl;
            return verify_result::Invalid;
        }

        if (proof.empty())
        {
            std::cerr << "Theorem " << label << " has no proof" << std::endl;
            return verify_result::Invalid;
        }

        tokens.pop(); // Discard $. token

        if (proof.find('?') != std::string::npos)
        {
            std::cerr << "Warning: Proof of theorem " << label
                      << " is incomplete" << std::endl;
            return verify_result::Incomplete;
        }

        std::vector<std::size_t> proofnumbers;
        proofnumbers.reserve(proof.size()); // Preallocate for efficiency
        verify_result result(getproofnumbers(label, proof, &proofnumbers));
        if (result != verify_result::Valid)
            return result;

        result = verifycompressedproof(label, assertion, labels, proofnumbers);
        if (result != verify_result::Valid)
            return result;
    }
    else
    {
        // Regular (uncompressed proof)
        std::vector<std::string> proof;
        bool incomplete(false);
        std::string token;
        while (!tokens.empty() && (token = tokens.front()) != "$.")
        {
            tokens.pop();
            proof.push_back(token);
            if (token == "?")
                incomplete = true;
            else if (token == label)
            {
                std::cerr << "Proof of theorem " << label
                          << " refers to itself" << std::endl;
                return verify_result::Invalid;
            }
            else if (assertions.find(token) == assertions.end()
                  && !isactivehyp(token))
            {
                std::cerr << "Proof of theorem " << label << " refers to "
                          << token << " which is not an active statement"
                          << std::endl;
                return verify_result::Invalid;
            }
        }

        if (tokens.empty())
        {
            std::cerr << "Unfinished $p statement " << label << std::endl;
            return verify_result::Invalid;
        }

        if (proof.empty())
        {
            std::cerr << "Theorem " << label << " has no proof" << std::endl;
            return verify_result::Invalid;
        }

        tokens.pop(); // Discard $. token

        if (incomplete)
        {
            std::cerr << "Warning: Proof of theorem " << label
                      << " is incomplete" << std::endl;
            return verify_result::Incomplete;
        }

        verify_result result(verifyregularproof(label, assertion, proof));
        if (result != verify_result::Valid)
            return result;
    }

    return verify_result::Valid;
}

// Parse $e statement.
verify_result parsee(std::string label)
{
    Expression newhyp;
    verify_result const result(readexpression('e', label, "$.", &newhyp));
    if (result != verify_result::Valid)
    {
        return result;
    }

    // Create new essential hypothesis
    hypotheses.insert(std::make_pair(label, std::make_pair(newhyp, false)));
    scopes.back().activehyp.push_back(label);

    return verify_result::Valid;
}

// Parse $a statement.
verify_result parsea(std::string label)
{
    Expression newaxiom;
    verify_result const result(readexpression('a', label, "$.", &newaxiom));
    if (result != verify_result::Valid)
    {
        return result;
    }

    constructassertion(label, newaxiom);

    return verify_result::Valid;
}

// Parse $f statement.
verify_result parsef(std::string label)
{
    if (tokens.empty())
    {
        std::cerr << "Unfinished $f statement" << label << std::endl;
        return verify_result::Invalid;
    }

    std::string type(tokens.front());

    if (constants.find(type) == constants.end())
    {
        std::cerr << "First symbol in $f statement " << label << " is "
                  << type << " which is not a constant" << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop();

    if (tokens.empty())
    {
        std::cerr << "Unfinished $f statement " << label << std::endl;
        return verify_result::Invalid;
    }

    std::string variable(tokens.front());
    if (!isactivevariable(variable))
    {
        std::cerr << "Second symbol in $f statement " << label << " is "
                  << variable << " which is not an active variable"
                  << std::endl;
        return verify_result::Invalid;
    }
    if (!getfloatinghyp(variable).empty())
    {
        std::cerr << "The variable " << variable
                  << " appears in a second $f statement "
                  << label << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop();

    if (tokens.empty())
    {
        std::cerr << "Unfinished $f statement" << label << std::endl;
        return verify_result::Invalid;
    }

    if (tokens.front() != "$.")
    {
        std::cerr << "Expected end of $f statement " << label
                  << " but found " << tokens.front() << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop(); // Discard $. token

    // Create new floating hypothesis
    Expression newhyp;
    newhyp.push_back(type);
    newhyp.push_back(variable);
    hypotheses.insert(std::make_pair(label, std::make_pair(newhyp, true)));
    scopes.back().activehyp.push_back(label);
    scopes.back().floatinghyp.insert(std::make_pair(variable, label));

    return verify_result::Valid;
}

// Parse labeled statement.
verify_result parselabel(std::string label)
{
    if (constants.find(label) != constants.end())
    {
        std::cerr << "Attempt to reuse constant " << label << " as a label"
                  << std::endl;
        return verify_result::Invalid;
    }

    if (variables.find(label) != variables.end())
    {
        std::cerr << "Attempt to reuse variable " << label << " as a label"
                  << std::endl;
        return verify_result::Invalid;
    }

    if (labelused(label))
    {
        std::cerr << "Attempt to reuse label " << label << std::endl;
        return verify_result::Invalid;
    }

    if (tokens.empty())
    {
        std::cerr << "Unfinished labeled statement" << std::endl;
        return verify_result::Invalid;
    }

    std::string const type(tokens.front());
    tokens.pop();

    verify_result result;
    if (type == "$p")
    {
        result = parsep(label);
    }
    else if (type == "$e")
    {
        result = parsee(label);
    }
    else if (type == "$a")
    {
        result = parsea(label);
    }
    else if (type == "$f")
    {
        result = parsef(label);
    }
    else
    {
        std::cerr << "Unexpected token " << type << " encountered"
                  << std::endl;
        return verify_result::Invalid;
    }

    return result;
}

// Parse $d statement.
verify_result parsed()
{
    std::set<std::string> dvars;

    std::string token;    

    while (!tokens.empty() && (token = tokens.front()) != "$.")
    { 
        tokens.pop();

        if (!isactivevariable(token))
        {
            std::cerr << "Token " << token << " is not an active variable, "
                      << "but was found in a $d statement" << std::endl;
            return verify_result::Invalid;
        }

        bool const duplicate(!dvars.insert(token).second);
        if (duplicate)
        {
            std::cerr << "$d statement mentions " << token << " twice"
                      << std::endl;
            return verify_result::Invalid;
        }
    }

    if (tokens.empty())
    {
        std::cerr << "Unterminated $d statement" << std::endl;
        return verify_result::Invalid;
    }

    if (dvars.size() < 2)
    {
        std::cerr << "Not enough items in $d statement" << std::endl;
        return verify_result::Invalid;
    }

    // Record it
    scopes.back().disjvars.push_back(dvars);

    tokens.pop(); // Discard $. token

    return verify_result::Valid;
}

// Parse $c statement.
verify_result parsec()
{
    if (scopes.size() > 1)
    {
        std::cerr << "$c statement occurs in inner block"
                  << std::endl;
        return verify_result::Invalid;
    }

    std::string token;
    bool listempty(true);
    while (!tokens.empty() && (token = tokens.front()) != "$.")
    { 
        tokens.pop();
        listempty = false;

        if (!ismathsymboltoken(token))
        {
            std::cerr << "Attempt to declare " << token
                      << " as a constant" << std::endl;
            return verify_result::Invalid;
        }
        if (variables.find(token) != variables.end())
        {
            std::cerr << "Attempt to redeclare variable " << token
                      << " as a constant" << std::endl;
            return verify_result::Invalid;
        }
        if (labelused(token))
        {
            std::cerr << "Attempt to reuse label " << token
                      << " as a constant" << std::endl;
            return verify_result::Invalid;
        }
        bool const alreadydeclared(!constants.insert(token).second);
        if (alreadydeclared)
        {
            std::cerr << "Attempt to redeclare constant " << token
                      << std::endl;
            return verify_result::Invalid;
        }
    }

    if (tokens.empty())
    {
        std::cerr << "Unterminated $c statement" << std::endl;
        return verify_result::Invalid;
    }

    if (listempty)
    {
        std::cerr << "Empty $c statement" << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop(); // Discard $. token

    return verify_result::Valid;
}

// Parse $v statement.
verify_result parsev()
{
    std::string token;
    bool listempty(true);
    while (!tokens.empty() && (token = tokens.front()) != "$.")
    { 
        tokens.pop();
        listempty = false;

        if (!ismathsymboltoken(token))
        {
            std::cerr << "Attempt to declare " << token
                      << " as a variable" << std::endl;
            return verify_result::Invalid;
        }
        if (constants.find(token) != constants.end())
        {
            std::cerr << "Attempt to redeclare constant " << token
                      << " as a variable" << std::endl;
            return verify_result::Invalid;
        }
        if (labelused(token))
        {
            std::cerr << "Attempt to reuse label " << token
                      << " as a variable" << std::endl;
            return verify_result::Invalid;
        }
        bool const alreadyactive(isactivevariable(token));
        if (alreadyactive)
        {
            std::cerr << "Attempt to redeclare active variable " << token
                      << std::endl;
            return verify_result::Invalid;
        }
        variables.insert(token);
        scopes.back().activevariables.insert(token);
    }

    if (tokens.empty())
    {
        std::cerr << "Unterminated $v statement" << std::endl;
        return verify_result::Invalid;
    }

    if (listempty)
    {
        std::cerr << "Empty $v statement" << std::endl;
        return verify_result::Invalid;
    }

    tokens.pop(); // Discard $. token

    return verify_result::Valid;
}

}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cerr << "Syntax: checkmm <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    bool const okay(readtokens(argv[1]));
    if (!okay)
        return EXIT_FAILURE;

    scopes.push_back(Scope());

    bool incompleteProofFound(false);

    while (!tokens.empty())
    {
        std::string const token(tokens.front());
        tokens.pop();

        verify_result result = verify_result::Valid;

        if (islabeltoken(token))
        {
            result = parselabel(token);
        }
        else if (token == "$d")
        {
            result = parsed();
        }
        else if (token == "${")
        {
            scopes.push_back(Scope());
        }
        else if (token == "$}")
        {
            scopes.pop_back();
            if (scopes.empty())
            {
                std::cerr << "$} without corresponding ${" << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if (token == "$c")
        {
            result = parsec();
        }
        else if (token == "$v")
        {
            result = parsev();
        }
        else if (token == "$[")
        {
            if (scopes.size() > 1)
            {
                std::cerr << "File inclusion command found in inner scope"
                    << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if (token == "$]")
        {
            // The closing delimiter occurs outside of any command.
            // Discard it.
        }
        else
        {
            std::cerr << "Unexpected token " << token << " encountered"
                      << std::endl;
            return EXIT_FAILURE;
        }
        if (result == verify_result::Invalid)
            return EXIT_FAILURE;
        else if (result == verify_result::Incomplete)
            incompleteProofFound = true;
    }

    if (scopes.size() > 1)
    {
        std::cerr << "${ without corresponding $}" << std::endl;
        return EXIT_FAILURE;
    }

    return incompleteProofFound ? EXIT_FAILURE : 0;
}
