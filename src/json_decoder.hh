<?hh // strict
namespace Lexidor\Json_Hack;
use namespace HH\Lib\Str;
use namespace HH\Lib\Map;
use type InvalidArgumentException;
use function json_decode;
use function json_last_error_msg;
use function trigger_error;
use function Lexidor\Json_Hack\__Private\hack_array_to_shape_preserve_vec;
use const JSON_BIGINT_AS_STRING;
use const JSON_FB_HACK_ARRAYS;

/**
 * Takes a string and attempts to `\json_decode` it.
 * If the decode succeeds the result if returned after $mapper is applied.
 * If mapper is not supplied the returned value will contain dicts AND vecs where applicable.
 * If mapper is supplied, the returned value will contain shapes AND vecs where applicable.
 *
 * @param $mapper may NOT throw and must return either a mapped result or null.
 * @throws `\InvalidArgumentException` if the decode failed.
 * @throws `\InvalidArgumentException` if $json was an empty string and 'empty_string_error' is true.
 * The option JSON_FB_HACK_ARRAYS is required since varray<_> is a KeyedContainer<_, _> at runtime.
 * This would break the implementation of `keyed_container_to_shape`.
 */
function json_decoder<T>(
    string $json,
    ?(function(mixed): T) $mapper = null,
    json_decoder_options_default $options = shape(),
): T {
    $options = _json_decoder_options_default($options);
    if (Str\length($json) === 0 && $options['empty_string_error']) {
        throw new InvalidArgumentException(
            Str\format('Empty string given as argument to %s', __FUNCTION__),
        );
    }

    $result = json_decode(
        $json,
        true,
        $options['decode_depth_limit'],
        JSON_BIGINT_AS_STRING | JSON_FB_HACK_ARRAYS,
    ) as dynamic;

    if ($result === null && $json !== 'null' && Str\length($json) !== 0) {
        throw new InvalidArgumentException(Str\format(
            'Json could not be decoded. Error: "%s"',
            json_last_error_msg(),
        ));
    }

    if ($mapper is nonnull && $result is KeyedContainer<_, _>) {
        $result = hack_array_to_shape_preserve_vec($result);
        return $mapper($result);
    }

    if ($mapper is nonnull) {
        trigger_error(
            'Json decode resulted in a scalar value, but a mapper was supplied. Returning null out of pity.',
        );
        /*HH_IGNORE_ERROR[4110] T is required to allow null.*/
        return null;
    }

    /*HH_FIXME[4110] No $mapper was given, so T is not bound. Returning a mixed type.*/
    return $result as mixed;
}
