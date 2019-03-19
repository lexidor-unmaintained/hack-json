<?hh // strict
namespace Lexidor\Json_Hack;
use namespace HH\Lib\Str;
use type InvalidArgumentException;
use function json_decode;
use function json_last_error_msg;
use const JSON_BIGINT_AS_STRING;
use const JSON_FB_HACK_ARRAYS;

/**
 * Takes a string and attempts to `\json_decode` it.
 * If the decode succeeds the result if returned after $mapper is applied.
 *
 * @param $mapper may NOT throw and must return either a mapped result or null.
 * @throws `\InvalidArgumentException` if the decode failed.
 * @throws `\InvalidArgumentException` if $json was an empty string and 'empty_string_error' is true.
 * The option JSON_FB_HACK_ARRAYS is required since varray<_> is a KeyedContainer<_, _> at runtime.
 * This would break the implementation of `keyed_container_to_shape`.
 */
function json_decoder<T>(
    string $json,
    (function(mixed): T) $mapper,
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
    );

    if ($result === null && $json !== 'null' && Str\length($json) !== 0) {
        throw new InvalidArgumentException(Str\format(
            'Json could not be decoded. Error: "%s"',
            json_last_error_msg(),
        ));
    }

    return $mapper($result);
}
