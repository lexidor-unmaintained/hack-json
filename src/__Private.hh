<?hh //strict
namespace Lexidor\Json_Hack\__Private;

type emptyshape = shape(...);

function keyed_container_to_shape<Tdontcare>(
    KeyedContainer<Tdontcare, mixed> $structure,
): emptyshape {
    $output = shape();
    foreach ($structure as $key => $value) {
        if ($value is dict<_, _>) {
            $value = keyed_container_to_shape($value);
        }
        /*HH_IGNORE_ERROR[4051] We are making a shape with NO know keys, so this is fine.*/
        $output[$key] = $value;
    }
    return $output;
}
