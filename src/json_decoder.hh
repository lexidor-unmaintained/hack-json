<?hh //strict
namespace Lexidor\Json_Hack;
use namespace HH\Lib\Str;
use type InvalidArgumentException;
use function json_decode as json_extract;
use function json_last_error_msg;
use const JSON_BIGINT_AS_STRING;

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

    $result = json_extract($json, true, 512, JSON_BIGINT_AS_STRING);

    if ($result === null && $json !== 'null' && Str\length($json) !== 0) {
        throw new InvalidArgumentException(Str\format(
            'Json could not be decoded. Error: "%s"',
            json_last_error_msg(),
        ));
    }

    return $mapper($result);
}
