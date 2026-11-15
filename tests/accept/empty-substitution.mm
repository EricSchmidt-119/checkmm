$( An example involving empty substitution. $)

$c |- wff * $.
$v x y $.

wx $f wff x $.
wy $f wff y $.

$( A wff is a sequence of asterisks. $)
we $a wff $.
wapp $a wff x * $.

${
  delete.1 $e |- x * y $.

  $( Inference: remove an asterisk. $)
  delete $a |- x y $.
$}

axiom $a |- * * * * * $.

$( Derive the empty sequence of asterisks,
   removing them from right to left. $)
theorem1 $p |- $=
    we
    we
      we wapp
      we
        we wapp wapp
        we
          we wapp wapp wapp
          we
            we wapp wapp wapp wapp
            we
            axiom
          delete
        delete
      delete
    delete
  delete
$.

$( Now remove them from left to right. $)
theorem2 $p |- $=
    we
    we
      we
      we wapp
        we
        we wapp wapp
          we
          we wapp wapp wapp
            we
            we wapp wapp wapp wapp
            axiom
          delete
        delete
      delete
    delete
  delete
$.

$( Now remove from the middle. $)
theorem3 $p |- $=
    we
    we
      we
      we wapp
        we wapp
        we wapp
          we wapp
          we wapp wapp
            we wapp wapp
            we wapp wapp
            axiom
          delete
        delete
      delete
    delete
  delete
$.

$( The same proofs in compressed format. $)

theorem1c $p |- $=
  ( we wapp axiom delete ) AAABZAEBZAFBZAGBACDDDDD $.

theorem2c $p |- $=
  ( we wapp axiom delete ) AAAABZAEBZAFBZAGBCDDDDD $.

theorem3c $p |- $=
  ( we wapp axiom delete ) AAAABZEEEEBZFFCDDDDD $.
