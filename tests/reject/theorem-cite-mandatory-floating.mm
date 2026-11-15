$( Test that a compressed proof citing a mandatory floating hypothesis is
   rejected. $)

$c |- var $.
$v x $.

vx $f var x $.

axiom $a |- x $.

theorem $p |- x $= ( axiom vx ) AB $.
