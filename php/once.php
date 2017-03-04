
<?php 

class OnlyOne {
  static $in = null;
  
  public static function inst() 
  {
    if (self::$in === null) {
       self::$in = new OnlyOne();
    }
    return self::$in;
  }

}

?>
