<?hh // strict
namespace Lexidor\Json_Hack\__Private;
use namespace HH\Lib\Vec;

type emptyshape = shape(...);

/**
 * Takes a dict<arraykey, mixed> and makes it into an `emptyshape`.
 * @throws `\StackOverflowException` if `$dict` is recursive.
 */
function dict_to_shape<Tdontcare>(dict<arraykey, mixed> $dict): emptyshape {
    $output = shape();
    foreach ($dict as $key => $value) {
        /*HH_IGNORE_ERROR[4051] We are making a shape with NO know keys, so this is fine.*/
        $output[$key] = hack_array_to_shape_preserve_vec($value);
    }
    return $output;
}

/**
 * Takes any argument and recursively makes dicts a shape.
 * The `$structure` may not contain anything other than (dict|vec|string|vec|bool|null).
 * If illegal types are contained within `$structure` they'll be left alone.
 */
function hack_array_to_shape_preserve_vec(mixed $structure): mixed {
    if ($structure is vec<_>) {
        return Vec\map($structure, $v ==> hack_array_to_shape_preserve_vec($v));
    } elseif ($structure is dict<_, _>) {
        return dict_to_shape($structure);
    }
    return $structure;
}
