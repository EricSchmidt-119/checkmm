$( Test that variables can be redeclared locally $)
$c setvar $.
${
  $v x $.
  vx $f setvar x $.
$}
${
  $v x $.
  vx_ $f setvar x $.
$}
