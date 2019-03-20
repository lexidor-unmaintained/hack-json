<?hh // strict
namespace Lexidor\Json_Hack;

use type DomainException;
use namespace HH\Lib\Str;

class JsonDecoder2<T1 as nonnull, T2 as nonnull> {

    public function __construct(
        private (function(KeyedContainer<arraykey, mixed>): ?T1) $mapper1,
        private (function(KeyedContainer<arraykey, mixed>): ?T2) $mapper2,
    ) {}

    <<Throwing(vec['DomainException', 'InvalidArgementException'])>>
    public function decode(
        string $json,
        json_decoder_options_default $options = shape(),
    ): (?T1, ?T2) {
        $options = _json_decoder_options_default($options);
        $mapper1 = $this->mapper1;
        $mapper2 = $this->mapper2;
        $mapping = $arg ==> tuple($mapper1($arg), $mapper2($arg));
        $decoded = json_decoder($json, $mapping, $options);

        if (
            $options['no_mapping_error'] &&
            $decoded[0] === null &&
            $decoded[1] === null
        ) {
            throw new DomainException(
                Str\format('All %d decodes have failed for %s', 2, __METHOD__),
            );
        }

        if (
            $options['decode_exclusive'] &&
            $decoded[0] !== null &&
            $decoded[1] !== null
        ) {
            throw new DomainException(
                'Decode exclusive ought to prevent you from having multiple succesful decodes, but both mappers returned a nonnull value.',
            );
        }

        return $decoded;
    }

}
