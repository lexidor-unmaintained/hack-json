<?hh // strict
namespace Lexidor\Json_Hack;

use type InvalidArgumentException;
use namespace HH\Lib\Str;
use namespace HH\Lib\Vec;
use namespace HH\Lib\C;


class JsonDecoder3<T1 as nonnull, T2 as nonnull, T3 as nonnull> {

    public function __construct(
        private (function(mixed): ?T1) $mapper1,
        private (function(mixed): ?T2) $mapper2,
        private (function(mixed): ?T3) $mapper3,
    ) {}

    public function decode(
        string $json,
        json_decoder_options_default $options = shape(),
    ): (?T1, ?T2, ?T3) {
        $options = _json_decoder_options_default($options);
        $mapper1 = $this->mapper1;
        $mapper2 = $this->mapper2;
        $mapper3 = $this->mapper3;
        $mapping =
            $arg ==> tuple($mapper1($arg), $mapper2($arg), $mapper3($arg));
        $decoded = json_decoder($json, $mapping, $options);

        if (
            $options['no_mapping_error'] &&
            $decoded[0] === null &&
            $decoded[1] === null &&
            $decoded[2] === null
        ) {
            throw new InvalidArgumentException(
                Str\format(
                    'All %d decodes have failed for %s',
                    3,
                    __FUNCTION__,
                ),
            );
        }

        if ($options['decode_exclusive']) {
            $succesfullDecodes = C\count(
                Vec\filter_nulls(vec[$decoded[0], $decoded[1], $decoded[2]]),
            );
            invariant(
                $succesfullDecodes < 2,
                'Decode exclusive ought to prevent you from having multiple succesful decodes, but %d mappers returned a nonnull value.',
                $succesfullDecodes,
            );
        }

        return $decoded;
    }

}
