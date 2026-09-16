$( Test that a DV violation with two variable substituted with
   the same variable is rejected. $)

$c |- var $.

$v x y z $.

vx $f var x $.
vy $f var y $.
vz $f var z $.

$d x y $.

axiom $a |- x y $.

theorem $p |- z z $= ( axiom ) AAB $.
