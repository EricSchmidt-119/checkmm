$( Test that a proof with a missing DV condition involving a
   dummy variable is rejected. $)

$c |- var $.

$v x y z $.

vx $f var x $.
vy $f var y $.
vz $f var z $.

axiom1 $a |- x y z $.

${
  $d x y z $.
  axiom2.1 $e |- x y z $.
  axiom2 $a |- x y $.
$}

${
  $d x y $. $d x z $.
  theorem $p |- x y $= vx vy vz vx vy vz axiom1 axiom2 $.
$}
