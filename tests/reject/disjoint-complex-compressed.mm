$( Test that a missing DV condition is caught with complex substitutions. $)

$c |- class = $.

$v A B C D E F G H $.

cA $f class A $.
cB $f class B $.
cC $f class C $.
cD $f class D $.
cE $f class E $.
cF $f class F $.
cG $f class G $.
cH $f class H $.

cat $a class A B $.

${
  $d A B $.

  axiom $a |- A = B $.
$}

${
  $d A E $. $d A F $. $d A G $. $d A H $.
  $d B E $. $d B F $.           $d B H $.
  $d C E $. $d C F $. $d C G $. $d C H $.
  $d D E $. $d D F $. $d D G $. $d D H $.

  theorem $p |- A B C D = E F G H $= ( cat axiom ) ABCDIIIEFGHIIIJ $.
$}
