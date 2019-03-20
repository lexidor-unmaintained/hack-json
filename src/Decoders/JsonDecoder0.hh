<?hh // strict
namespace Lexidor\Json_Hack;

/**
 * For documentation see JsonDecoderREADME.md
 */
class JsonDecoder0<T as nonnull> {

    public function __construct() {}

    <<Throws(vec['DomainException', 'InvalidArgementException'])>>
    public function decode(
        string $json,
        json_decoder_options_default $options = shape(),
    ): ?T {
        $options = _json_decoder_options_default($options);
        return json_decoder($json, null, $options);
    }

}
