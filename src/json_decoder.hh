<?hh // strict
namespace Lexidor\Json_Hack;
use namespace HH\Lib\Str;
use type DomainException;
use type InvalidArgumentException;
use function json_decode;
use function json_last_error_msg;
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
 * @throws `\DomainException` if $json was an empty string and 'empty_string_error' is true.
 * The option JSON_FB_HACK_ARRAYS is required since varray<_> is a KeyedContainer<_, _> at runtime and there is no difference between darray<_, _> and varray<_>.
 */
<<Throws(vec['DomainException', 'InvalidArgementException'])>>
function json_decoder<T>(
    string $json,
    ?(function(KeyedContainer<arraykey, mixed>): T) $mapper = null,
    json_decoder_options_default $options = shape(),
): T {
    $options = _json_decoder_options_default($options);
    if (Str\length($json) === 0 && $options['empty_string_error']) {
        throw new DomainException(
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
        /*HH_IGNORE_ERROR[4110] The KeyedContainer interface does not specify that keys are arraykeys, but hack arrays require that.*/
        return $mapper($result);
    }

    if ($mapper is nonnull) {
        if ($options['scalar_with_mapper_error']) {
            throw new DomainException(Str\format(
                'Explicit error for scalar decode when KeyedContainer<_, _> was expected, got %s',
                $result === null
                    ? 'null'
                    : (
                        $result is bool
                            ? ($result ? 'true' : 'false')
                            : (string)$result
                    ),
            ));
        }
        /*HH_IGNORE_ERROR[4110] T is required to allow null.*/
        return null;
    }

    /*HH_FIXME[4110] No $mapper was given, so T is not bound. Returning a mixed type.*/
    return $result as mixed;
}
