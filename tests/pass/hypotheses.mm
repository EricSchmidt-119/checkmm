$( Test handling of $e and $f statements. $)

$c A B C D E $.

$( *** Section 1: Just $e hypotheses *** $)

${
  axiom1 $a A $.

  ahyp1 $e A $.

  axiom2 $a B $.

  ahyp2 $e B $.

  axiom3 $a C $.
$}

theorem0  $p C $= axiom1 axiom1 axiom2 axiom3 $.
theorem0c $p C $= ( axiom1 axiom2 axiom3 ) AABC $.

${
  theorem1  $p A $= axiom1 $.
  theorem1c $p A $= ( axiom1 ) A $.

  thyp1 $e A $.

  theorem2  $p B $= thyp1 axiom2 $.
  theorem2c $p B $= ( axiom2 ) AB $.

  thyp2 $e B $.

  theorem3  $p C $= thyp1 thyp2 axiom3 $.
  theorem3c $p C $= ( axiom3 ) ABC $.
$}

$( *** Section 2: $f and $e hypotheses *** $)

$c |- $.

$v x  y  z  $.
$v x2 y2 z2 $.

${
  vx $f A x $. vx2 $f A x2 $.
  vy $f B y $. vy2 $f B y2 $.
  vz $f C z $. vz2 $f C z2 $.

  axiom4 $a |- x y z $.

  theorem4  $p |- x2 y2 z2 $= vx2 vy2 vz2 axiom4 $.
  theorem4c $p |- x2 y2 z2 $= ( axiom4 ) ABCD $.

  hyp3 $e |- z y x $.

  axiom5 $a |- y y y $.

  theorem5  $p |- y y y $= vx vy vz hyp3 axiom5 $.
  theorem5c $p |- y y y $= ( axiom5 ) ABCDE $.
$}

${
  vy_ $f B y $. vy2_ $f B y2 $.
  vx_ $f A x $. vx2_ $f A x2 $.
  vz_ $f C z $. vz2_ $f C z2 $.

  hyp4 $e |- z y2 x $.

  $( The $f hypotheses in this block are in a different order from
     the previous. In the proof of theorem6, we need to cite them
     in the order they appear to theorem5.
  $)
  theorem6  $p |- y2 y2 y2 $= vx_ vy2_ vz_ hyp4 theorem5 $.
  theorem6c $p |- y2 y2 y2 $= ( theorem5 ) BACDE $.
$}

${
  vz__ $f C z $. vz2__ $f C z2 $.
  vy__ $f B y $. vy2__ $f B y2 $.
  vx__ $f A x $. vx2__ $f A x2 $.

  ${
    a6.1 $e |- y y y $.
    a6.2 $e |- y2 y2 y2 $.

    axiom6 $a |- y y y y2 y2 y2 $.
  $}

  ${
    t7.1 $e |- z y x $.
    t7.2 $e |- z2 y2 x2 $.

    $( In theorem7, we use both theorem5 and theorem6, which
       have a $f order different from each other and from
       this block. In the compressed proof, all three orders
       need to be handled correctly. $)
    theorem7 $p |- y2 y2 y2 y y y $=
      vy2__ vy__
        vx2__ vy2__  vz2__ t7.2 theorem5
        vy__  vx__   vz__  t7.1 theorem6
      axiom6 $.

    theorem7c $p |- y2 y2 y2 y y y $= ( theorem5 theorem6 axiom6 )
      DC
        FDBHI
        CEAGJ
      K $.
  $}
$}

$( *** Section 3: $f and $e hypotheses interspersed *** $)

$v p $.
$v q $.

${
  vp   $f A p $.
  a7.1 $e |- p $.
  vq   $f A q $.
  a7.2 $e |- q $.
  axiom7 $a |- p q $.
$}

$v p2 $.
$v q2 $.

${
  vp2  $f A p2 $.
  vq2  $f A q2 $.
  t8.1 $e |- p2 $.
  t8.2 $e |- q2 $.

  theorem8  $p |- p2 q2 $= vp2 t8.1 vq2 t8.2 axiom7 $.
  theorem8c $p |- p2 q2 $= ( axiom7 ) ACBDE $.
$}

$( *** Section 4: Nesting *** $)

h1. $e A $.

${
  h2 $e B $.
  ${
    h3 $e C $.
    ${
      h4 $e D $.
      ${
        h5 $e E $.
        a5 $a |- E $.
      $}
      a4 $a |- D $.
    $}
    a3 $a |- C $.
  $}
  a2 $a |- B $.
$}

a1 $a |- A $.

t1  $p |- A $= h1. a1 $.
t1c $p |- A $= ( a1 ) AB $.

${
  h2. $e B $.
  t2  $p |- B $= h1. h2. a2 $.
  t2c $p |- B $= ( a2 ) ABC $.
  ${
    h3. $e C $.
    t3  $p |- C $= h1. h2. h3. a3 $.
    t3c $p |- C $= ( a3 ) ABCD $.
    ${
      h4. $e D $.
      t4  $p |- D $= h1. h2. h3. h4. a4 $.
      t4c $p |- D $= ( a4 ) ABCDE $.
      ${
        h5. $e E $.
        t5  $p |- E $= h1. h2. h3. h4. h5. a5 $.
        t5c $p |- E $= ( a5 ) ABCDEF $.
      $}
      t4_  $p |- D $= h1. h2. h3. h4. a4 $.
      t4_c $p |- D $= ( a4 ) ABCDE $.
    $}
    t3_  $p |- C $= h1. h2. h3. a3 $.
    t3_c $p |- C $= ( a3 ) ABCD $.
  $}
  t2_  $p |- B $= h1. h2. a2 $.
  t2_c $p |- B $= ( a2 ) ABC $.
$}

t1_  $p |- A $= h1. a1 $.
t1_c $p |- A $= ( a1 ) AB $.
