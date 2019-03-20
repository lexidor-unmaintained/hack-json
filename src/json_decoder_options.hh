<?hh // strict

namespace Lexidor\Json_Hack;

/**
 * A shape that contains all the options that can be used of `json_decoder` and `JsonDecoder(%d)`.
 * All options are required to be filled.
 */
type json_decoder_options = shape(
    'empty_string_error' => bool,
    'no_mapping_error' => bool,
    'decode_exclusive' => bool,
    'decode_depth_limit' => int,
    'scalar_with_mapper_error' => bool,
);

/**
 * A shape that contains all the options that can be used of `json_decoder` and `JsonDecoder(%d)`.
 * An option that is not set will be replaced by the default.
 */
type json_decoder_options_default = shape(
    //Should the json_decoder function throw an InvalidArgumentException when an empty string is provided?
    ?'empty_string_error' => bool,
    //Should the JsonDecoder(%d) throw an InvalidArgumentException when all mappers return a null? (Has no effect when %d === 0)
    ?'no_mapping_error' => bool,
    //Should the JsonDecoder(%d) require that nomore than one decode operation returns a nonnull value? (Has no effect is %d <= 1)
    ?'decode_exclusive' => bool,
    //How deep should json_decoder try to go before giving up?
    ?'decode_depth_limit' => int,
    //Should json_decoder throw an InvalidArgumentException if the json decodes to a scalar and a mapper was provided? (Has no effect when %d === 0)
    ?'scalar_with_mapper_error' => bool,
);

/**
 * Transforms an `json_decoder_options_default` into a `json_decoder_options` by applying defaults.
 */
function _json_decoder_options_default(
    json_decoder_options_default $options,
): json_decoder_options {
    $options['empty_string_error'] ??= true;
    $options['no_mapping_error'] ??= true;
    $options['decode_exclusive'] ??= false;
    $options['decode_depth_limit'] ??= 512;
    $options['scalar_with_mapper_error'] ??= false;
    return $options;
}
