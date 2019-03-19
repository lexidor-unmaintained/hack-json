<?hh //strict

namespace Lexidor\Json_Hack;

/**
 * A shape that contains all the options that can be used of `json_decoder` and `JsonDecoder(%d+)`.
 * All options are required to be filled.
 */
type json_decoder_options = shape(
    'empty_string_error' => bool,
    'no_mapping_error' => bool,
    'decode_exclusive' => bool,
    'decode_depth_limit' => int,
);

/**
 * A shape that contains all the options that can be used of `json_decoder` and `JsonDecoder(%d+)`.
 * A option that is not set may be replaced by the default.
 */
type json_decoder_options_default = shape(
    ?'empty_string_error' => bool,
    ?'no_mapping_error' => bool,
    ?'decode_exclusive' => bool,
    ?'decode_depth_limit' => int,
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
    return $options;
}
