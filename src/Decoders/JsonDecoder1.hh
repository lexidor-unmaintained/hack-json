<?hh //strict
namespace Lexidor\Json_Hack;

use namespace HH\Lib\Str;
use type InvalidArgumentException;

/**
 * For documentation see JsonDecoderREADME.md
 */
class JsonDecoder1<T as nonnull> {

    public function __construct(private (function(mixed): ?T) $mapper1) {}

    public function decode(
        string $json,
        json_decoder_options_default $options = shape(),
    ): ?T {
        $options = _json_decoder_options_default($options);
        $decoded = json_decoder($json, $this->mapper1, $options);
        if ($options['no_mapping_error'] && $decoded === null) {
            throw new InvalidArgumentException(
                Str\format(
                    'All decodes %d have failed for %s',
                    1,
                    __FUNCTION__,
                ),
            );
        }
        return json_decoder($json, $this->mapper1, $options);
    }

}
