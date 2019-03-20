<?hh // strict
namespace Lexidor\Json_Hack\__Private;
use namespace HH\Lib\Vec;
use namespace HH\Lib\Dict;

type emptyshape = shape(...);

/**
 * Takes a KeyedContainer and makes it into an `emptyshape`.
 * @throws `\StackOverflowException` if $structure is recursive.
 */
function dict_to_shape<Tdontcare>(dict<arraykey, mixed> $dict): emptyshape {
    $output = shape();
    foreach ($dict as $key => $value) {
        /*HH_IGNORE_ERROR[4051] We are making a shape with NO know keys, so this is fine.*/
        $output[$key] = $value;
    }
    return $output;
}

function hack_array_to_shape_preserve_vec(mixed $structure): mixed {
    if ($structure is vec<_>) {
        return Vec\map($structure, $v ==> hack_array_to_shape_preserve_vec($v));
    } elseif ($structure is dict<_, _>) {
        return Dict\map($structure, $v ==> hack_array_to_shape_preserve_vec($v))
            |> dict_to_shape($$);
    }
    return $structure;
}
