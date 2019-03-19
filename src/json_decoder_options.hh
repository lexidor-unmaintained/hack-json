<?hh //strict

namespace Lexidor\Json_Hack;

type json_decoder_options = shape(
    'empty_string_error' => bool,
    'no_mapping_error' => bool,
    'decode_exclusive' => bool,
    'decode_depth_limit' => int,
);

type json_decoder_options_default = shape(
    ?'empty_string_error' => bool,
    ?'no_mapping_error' => bool,
    ?'decode_exclusive' => bool,
    ?'decode_depth_limit' => int,
);


function _json_decoder_options_default(
    json_decoder_options_default $options,
): json_decoder_options {
    $options['empty_string_error'] ??= true;
    $options['no_mapping_error'] ??= true;
    $options['decode_exclusive'] ??= false;
    $options['decode_depth_limit'] ??= 512;
    return $options;
}
