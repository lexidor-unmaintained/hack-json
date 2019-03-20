<?hh //strict
namespace Lexidor\Json_Hack;

use type Facebook\TypeAssert\IncorrectTypeException;
use type Facebook\TypeAssert\TypeCoercionException;
use type TypeAssertionException;


function null_no_type_error<T>(
    (function(KeyedContainer<arraykey, mixed>): T) $func,
): (function(KeyedContainer<arraykey, mixed>): ?T) {
    return ($arg) ==> {
        try {
            return $func($arg);
        } catch (TypeAssertionException $_) {
            return null;
        } catch (IncorrectTypeException $_) {
            return null;
        } catch (TypeCoercionException $_) {
            return null;
        }
    };
}
