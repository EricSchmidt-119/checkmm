$( Test disjoint variable conditions $)

$c |- class = _V $.

$v A B C D  $.

cA $f class A $.
cB $f class B $.
cC $f class C $.
cD $f class D $.

ccv $a class _V $.

cat $a class A B $.

${
  $d A B $.
  $( Prove a theorem with an unnecessary DV condition. $)
  catt  $p class A B $= cA cB cat $.
  cattc $p class A B $= ( cat ) ABC $.
$}

$( Create two versions of the same axiom, one with a DV condition,
   the other without. $)
${
  nd-axiom $a |- A = B $.

  $d A B $.

  axiom $a |- A = B $.
$}

${
  $d C D $.
  theorem1  $p |- C = D $= cC cD axiom $.
  theorem1c $p |- C = D $= ( axiom ) ABC $.
$}

${
  $d A B $.
  theorem2  $p |- B = A $= cB cA axiom $.
  theorem2c $p |- B = A $= ( axiom ) BAC $.
$}

${
  $d B C $.
  theorem3  $p |- B = C $= cB cC axiom $.
  theorem3c $p |- B = C $= ( axiom ) ABC $.
$}

${
  $d C A D $.
  theorem4  $p |- C = D $= cC cD axiom $.
  theorem4c $p |- C = D $= ( axiom ) ABC $.
$}

${
  $d A D B C $.
  theorem5  $p |- A = B $= cA cB axiom $.
  theorem5c $p |- A = B $= ( axiom ) ABC $.
$}

${
  $d A B C D $.
  theorem6  $p |- B = C $= cB cC axiom $.
  theorem6c $p |- B = C $= ( axiom ) ABC $.
$}

${
  theorem7  $p |- _V = _V $= ccv ccv axiom $.
  theorem7c $p |- _V = _V $= ( ccv axiom ) AAB $.
$}

${
  $d C D $.
  theorem8  $p |- C C = D D $= cC cC cat cD cD cat axiom $.
  theorem8c $p |- C C = D D $= ( cat axiom ) AACBBCD $.
$}

${
  $d A C $. $d A D $. $d B C $. $d B D $.
  theorem9  $p |- A B = C D $= cA cB cat cC cD cat axiom $.
  theorem9c $p |- A B = C D $= ( cat axiom ) ABECDEF $.
$}

${
  $d A B C D $.
  theorem9A  $p |- A B = C D $= cA cB cat cC cD cat axiom $.
  theorem9Ac $p |- A B = C D $= ( cat axiom ) ABECDEF $.
$}

${
  $d C D $.
  theorem10  $p |- C _V = D _V $= cC ccv cat cD ccv cat axiom $.
  theorem10c $p |- C _V = D _V $= ( ccv cat axiom ) ACDBCDE $.
$}

$( Now prove the same theorems without a DV condition. $)

nd-theorem1  $p |- C = D $= cC cD nd-axiom $.
nd-theorem1c $p |- C = D $= ( nd-axiom ) ABC $.

nd-theorem2  $p |- B = A $= cB cA nd-axiom $.
nd-theorem2c $p |- B = A $= ( nd-axiom ) BAC $.

nd-theorem3  $p |- B = C $= cB cC nd-axiom $.
nd-theorem3c $p |- B = C $= ( nd-axiom ) ABC $.

nd-theorem4  $p |- C = D $= cC cD nd-axiom $.
nd-theorem4c $p |- C = D $= ( nd-axiom ) ABC $.

nd-theorem5  $p |- A = B $= cA cB nd-axiom $.
nd-theorem5c $p |- A = B $= ( nd-axiom ) ABC $.

nd-theorem6  $p |- B = C $= cB cC nd-axiom $.
nd-theorem6c $p |- B = C $= ( nd-axiom ) ABC $.

nd-theorem7  $p |- _V = _V $= ccv ccv nd-axiom $.
nd-theorem7c $p |- _V = _V $= ( ccv nd-axiom ) AAB $.

nd-theorem8  $p |- C C = D D $= cC cC cat cD cD cat nd-axiom $.
nd-theorem8c $p |- C C = D D $= ( cat nd-axiom ) AACBBCD $.

nd-theorem9  $p |- A B = C D $= cA cB cat cC cD cat nd-axiom $.
nd-theorem9c $p |- A B = C D $= ( cat nd-axiom ) ABECDEF $.

nd-theorem10  $p |- C _V = D _V $= cC ccv cat cD ccv cat nd-axiom $.
nd-theorem10c $p |- C _V = D _V $= ( ccv cat nd-axiom ) ACDBCDE $.

$( Theorems with dummy variables $)

${
  eqtri.1 $e |- A = B $.
  eqtri.2 $e |- B = C $.
  eqtri $a |- A = C $.
$}

${
  $d A C $.
  theorem11  $p |- A = A $= cA cC cA cA cC axiom cC cA axiom eqtri $.
  theorem11c $p |- A = A $= ( cC axiom eqtri ) ABAABCBACD $.
$}

${
  nd-theorem11  $p |- A = A $= cA cC cA cA cC nd-axiom cC cA nd-axiom eqtri $.
  nd-theorem11c $p |- A = A $= ( cC nd-axiom eqtri ) ABAABCBACD $.
$}

${
  theorem12  $p |- C = C $= cC theorem11 $.
  theorem12c $p |- C = C $= ( theorem11 ) AB $.
$}

${
  theorem13  $p |- C = C $= cC theorem11c $.
  theorem13c $p |- C = C $= ( theorem11c ) AB $.
$}

${
  $d A C $. $d C D $. $d D B $.
  theorem14 $p |- A = B $=
    cA cD cB
    cA cC cD
    cA cC axiom    $( |- A = C $)
    cC cD axiom    $( |- C = D $)
    eqtri          $( |- A = D $)
    cD cB axiom    $( |- D = B $)
    eqtri $.       $( |- A = B $) 

  theorem14c $p |- A = B $= ( cC cD axiom eqtri ) ADBACDACECDEFDBEF $.
$}

${
  theorem15  $p |- C = D $= cC cD theorem14 $.
  theorem15c $p |- C = D $= ( theorem14 ) ABC $. 
$}

${
  theorem16  $p |- C = D $= cC cD theorem14c $.
  theorem16c $p |- C = D $= ( theorem14c ) ABC $. 
$}

$( Test with nested scopes $)

$d A C $.
${
  $d B C $.
  ${
    $d A D $.
    ${
      $d B D $.
      
      nested-theorem9  $p |- A B = C D $= cA cB cat cC cD cat axiom $.
      nested-theorem9c $p |- A B = C D $= ( cat axiom ) ABECDEF $.
    $}
  $}
$}
