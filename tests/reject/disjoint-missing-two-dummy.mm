$( Test that a proof with a missing DV condition involving two
   dummy variables is rejected. $)

$c |- var $.

$v x y z w $.

vx $f var x $.
vy $f var y $.
vz $f var z $.
vw $f var w $.

axiom1 $a |- x y z w $.

${
  $d x y z w $.
  axiom2.1 $e |- x y z w $.
  axiom2 $a |- x y $.
$}

${
  $d x y $.
  $d x z $. $d x w $.
  $d y z $. $d y w $.
  theorem $p |- x y $= vx vy vz vw vx vy vz vw axiom1 axiom2 $.
$}
