$( Test substitution in proofs. $)

$c |- var # ; = $.

$v x y z w $.

vx $f var x $.
vy $f var y $.
vz $f var z $.
vw $f var w $.

vnumb $a var # x $.
vsemi $a var ; x y $.

axiom $a |- x = y $.

$( Substiute variables with themselves. $)

theorem1  $p |- x = y $= vx vy axiom $.
theorem1c $p |- x = y $= ( axiom ) ABC $.

$( Substitute both variables with one of them. $)
theorem2  $p |- x = x $= vx vx axiom $.
theorem2c $p |- x = x $= ( axiom ) AAB $.

$( Substitute x with # x $)
theorem3  $p |- # x = y $= vx vnumb vy axiom $.
theorem3c $p |- # x = y $= ( axiom vnumb ) ADBC $.

$( Substitute x with # y $)
theorem4  $p |- # y = y $= vy vnumb vy axiom $.
theorem4c $p |- # y = y $= ( axiom vnumb ) ACAB $.

$( Substitute x with z $)
theorem5  $p |- z = y $= vz vy axiom $.
theorem5c $p |- z = y $= ( axiom ) BAC $.

$( Substitute x with # z $)
theorem6  $p |- # z = y $= vz vnumb vy axiom $.
theorem6c $p |- # z = y $= ( axiom vnumb ) BDAC $.

$( Substitute x with # w and y with # # z $)
theorem7  $p |- # w = # # z $= vw vnumb vz vnumb vnumb axiom $.
theorem7c $p |- # w = # # z $= ( axiom vnumb ) BDADDC $.

$( Substitute x with ; x y $)
theorem8  $p |- ; x y = y $= vx vy vsemi vy axiom $.
theorem8c $p |- ; x y = y $= ( axiom vsemi ) ABDBC $.

$( Substitute x with ; x x $)
theorem9  $p |- ; x x = y $= vx vx vsemi vy axiom $.
theorem9c $p |- ; x x = y $= ( axiom vsemi ) AADBC $.

$( Substitute x with ; x w and y with ; z y $)
theoremA  $p |- ; x w = ; z y $= vx vw vsemi vz vy vsemi axiom $.
theoremAc $p |- ; x w = ; z y $= ( axiom vsemi ) ADFCBFE $.

$( Substitute x with ; x # w and y with ; # z y $)
theoremB  $p |- ; x # w = ; # z y $= vx vw vnumb vsemi vz vnumb vy vsemi axiom $.
theoremBc $p |- ; x # w = ; # z y $= ( axiom vnumb vsemi ) ADFGCFBGE $.

