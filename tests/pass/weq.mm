$( Reproduce ~ weq from set.mm $)

$c wff setvar class = $.

$v x y A B $.

vx $f setvar x $.
vy $f setvar y $.

cA $f class A $.
cB $f class B $.

cv $a class x $.

wceq $a wff A = B $.

weq  $p wff x = y $= vx cv vy cv wceq $.
weqc $p wff x = y $= ( cv wceq ) ACBCD $.
