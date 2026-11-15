$( Test disjoint variable conditions $)

$c |- class = _V $.

$v A B C D E F G H $.

cA $f class A $.
cB $f class B $.
cC $f class C $.
cD $f class D $.
cE $f class E $.
cF $f class F $.
cG $f class G $.
cH $f class H $.

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
  $d A E $. $d A F $. $d A G $. $d A H $.
  $d B E $. $d B F $. $d B G $. $d B H $.
  $d C E $. $d C F $. $d C G $. $d C H $.
  $d D E $. $d D F $. $d D G $. $d D H $.

  theorem10  $p |- A B C D = E F G H $=
    cA cB cC cD cat cat cat cE cF cG cH cat cat cat axiom $.
  theorem10c $p |- A B C D = E F G H $= ( cat axiom ) ABCDIIIEFGHIIIJ $.
$}

${
  $d A B C D E F G H $.
  theorem10A  $p |- A B C D = E F G H $=
    cA cB cC cD cat cat cat cE cF cG cH cat cat cat axiom $.
  theorem10Ac $p |- A B C D = E F G H $= ( cat axiom ) ABCDIIIEFGHIIIJ $.
$}

${
  $d C D $.
  theorem11  $p |- C _V = D _V $= cC ccv cat cD ccv cat axiom $.
  theorem11c $p |- C _V = D _V $= ( ccv cat axiom ) ACDBCDE $.
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
  theorem12  $p |- A = A $= cA cC cA cA cC axiom cC cA axiom eqtri $.
  theorem12c $p |- A = A $= ( cC axiom eqtri ) ABAABCBACD $.
$}

${
  nd-theorem12  $p |- A = A $= cA cC cA cA cC nd-axiom cC cA nd-axiom eqtri $.
  nd-theorem12c $p |- A = A $= ( cC nd-axiom eqtri ) ABAABCBACD $.
$}

${
  theorem13  $p |- C = C $= cC theorem12 $.
  theorem13c $p |- C = C $= ( theorem12 ) AB $.
$}

${
  theorem14  $p |- C = C $= cC theorem12c $.
  theorem14c $p |- C = C $= ( theorem12c ) AB $.
$}

${
  $d A C $. $d C D $. $d D B $.
  theorem15 $p |- A = B $=
    cA cD cB
    cA cC cD
    cA cC axiom    $( |- A = C $)
    cC cD axiom    $( |- C = D $)
    eqtri          $( |- A = D $)
    cD cB axiom    $( |- D = B $)
    eqtri $.       $( |- A = B $)

  theorem15c $p |- A = B $= ( cC cD axiom eqtri ) ADBACDACECDEFDBEF $.
$}

${
  theorem16  $p |- C = D $= cC cD theorem15 $.
  theorem16c $p |- C = D $= ( theorem15 ) ABC $.
$}

${
  theorem17  $p |- C = D $= cC cD theorem15c $.
  theorem17c $p |- C = D $= ( theorem15c ) ABC $.
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
