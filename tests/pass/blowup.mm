$( An example that would blow up to many steps if converted to
   an uncompressed proof. $)

$c wff * $.
$v x $.

vx $f wff x $.

wast $a wff * $.

${
  wapp.1 $e wff x $.

  $( A syntax construction rule with a redundant hypothesis. Using it
     requires proving ` wff x ` twice. $)
  wapp $a wff x * $.
$}

$( The following theorem has 2^64 - 1 uncompressed proof steps. $)
64asterisks $p wff
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
    * * * * * * * *
  $= ( wast wapp )
    A    ZCB  ZDB  ZEB  ZFB  ZGB  ZHB  ZIB
    ZJB  ZKB  ZLB  ZMB  ZNB  ZOB  ZPB  ZQB
    ZRB  ZSB  ZTB  ZUAB ZUBB ZUCB ZUDB ZUEB
    ZUFB ZUGB ZUHB ZUIB ZUJB ZUKB ZULB ZUMB
    ZUNB ZUOB ZUPB ZUQB ZURB ZUSB ZUTB ZVAB
    ZVBB ZVCB ZVDB ZVEB ZVFB ZVGB ZVHB ZVIB
    ZVJB ZVKB ZVLB ZVMB ZVNB ZVOB ZVPB ZVQB
    ZVRB ZVSB ZVTB ZWAB ZWBB ZWCB ZWDB ZWEB
  $.
