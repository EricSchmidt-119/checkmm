$( Test that a missing DV condition is rejected. $)

$c |- var $.

$v x y z w $.

vx $f var x $.
vy $f var y $.
vz $f var z $.
vw $f var w $.

${
  $d x y $.
  $d z w $.
  axiom $a |- x y $.
$}

${
  $d x y $.
  theorem $p |- z w $= vz vw axiom $.
$}
