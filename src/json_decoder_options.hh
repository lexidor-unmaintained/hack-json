<?hh //strict

namespace Lexidor\Json_Hack;

type json_decoder_options = shape(
    'empty_string_error' => bool,
);

type json_decoder_options_default = shape(
    ?'empty_string_error' => bool,
);


function _json_decoder_options_default(
    json_decoder_options_default $options,
): json_decoder_options {
    $options['empty_string_error'] = $options['empty_string_error'] ?? true;
    return $options;
}
