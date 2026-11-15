$c wff |- T $.
$v ph $.
wph $f wff ph $.

axiom $a |- T $.

$( A single-step proof citing an axiom. $)
theorem1  $p |- T $= axiom $.
theorem1c $p |- T $= ( axiom ) A $.

$( A single-step proof citing a theorem. $)
theorem2  $p |- T $= theorem1 $.
theorem2c $p |- T $= ( theorem1 ) A $.

hyp $e |- ph ph $.

$( A single-step proof citing a floating hypothesis. $)
theorem3  $p wff ph $= wph $.
theorem3c $p wff ph $= ( ) A $.

$( A single-step proof citing an essential hypothesis. $)
theorem4  $p |- ph ph $= hyp $.
theorem4c $p |- ph ph $= ( ) B $.
