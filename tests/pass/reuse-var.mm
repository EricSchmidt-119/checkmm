$( Test that the same variable can be declared and used in multiple
   inner scopes. $)

$c |- var = $.

$v x $.

${
  $v y $.
  
  vx.1 $f var x $.
  vy.1 $f var y $.
  
  axiom $a |- x = y $.
$}

${
  $v y z $.
    
  vx.2 $f var y $.

  vy.2 $f var x $.

  theorem  $p |- y = x $= vx.2 vy.2 axiom $.
  theoremc $p |- y = x $= ( theorem ) ABC $.
$}
