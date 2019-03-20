<?hh //strict
namespace Lexidor\Json_Hack\Example;

use type Lexidor\Json_Hack\JsonDecoder3;
use namespace HH\Lib\Str;
use function trigger_error;
use type DomainException;
use type InvalidArgumentException;
use namespace Facebook\TypeAssert;
use namespace Facebook\TypeSpec;
use type Facebook\TypeAssert\IncorrectTypeException;
use function Lexidor\Json_Hack\null_no_type_error;


<<__EntryPoint>>
function advanced_example(): noreturn {
    require_once __DIR__.'/vendor/hh_autoload.hh';

    $endpoint = 'https://api.github.com/...';

    //Let's pretend to curl an api with an iterable field "repos".
    $apiResponse = \json_encode(
        [
            'success' => true,
            'username' => 'Lexidor',
            'repos' => ['hack-json', 'empty-hack-project'],
        ],
    );

    /*
     * We expect one of three results:
     * {"success" => bool, "username" => string, "repos" => Traversable<string>}
     * {"success" => bool, "reason" => string}
     * {"success" => bool, "timeout" => int}
     * And each of them might receive more fields in the future without breaking BC.
     */

    /*
     * Since ?as shape('key' => Traversable<_>) is both, not very useful and not allowed,
     * We'll need hhvm/type-assert and define a function that checks the response including the Traversable<_>
     */

    //A spec for a vec of strings (vec<string>).
    $repoSpec = TypeSpec\vec(TypeSpec\string());


    $normalSpec = (
        KeyedContainer<arraykey, mixed> $arg,
    ): ?shape(
        "success" => bool,
        "username" => string,
        "repos" => vec<string>,
        ...
    ) ==> {
        $arg = $arg as shape(
            "success" => bool,
            "username" => string,
            "repos" => mixed,
            ...
        );
        $arg['repos'] = $repoSpec->assertType($arg['repos']);
        return $arg;
    };

    $decoder = new JsonDecoder3(
        null_no_type_error($normalSpec),
        $error ==> $error ?as shape("success" => bool, "reason" => string, ...),
        $timeout ==> $timeout ?as
            shape("success" => bool, "timeout" => int, ...),
    );

    /*
     * We expect at least one of these three results and if none match an exception should be thrown.
     * This is the default, but you could turn it off by setting 'no_mapping_error' to false.
     * We expect that no more than one of the results are valid. If more than one is valid, the API has changed.
     * By default this is not an exception, but we will turn this on by using 'decode_exclusive'.
     * This will force 0 or 1 mappers to work.
     * By using these two together we enforce that 1 and only one must succeed.
     */

    try {
        list($normal, $error, $timeout) =
            $decoder->decode($apiResponse, shape('decode_exclusive' => true));
    } catch (DomainException $e) {
        \trigger_error(Str\format(
            'API %s: did not meet expectations. Error: %s',
            $endpoint,
            $e->__toString(),
        ));
        exit(1);
    } catch (InvalidArgumentException $e) {
        trigger_error(Str\format(
            'Json from endpoint %s could not be decoded. Error: %s',
            $endpoint,
            $e->__toString(),
        ));
        exit(1);
    }

    if ($normal !== null) {
        //The result was (as expected) ['success' => 1, 'username' => 'Lexidor', 'repo' => 'hack-json']
        //These fields are now available using autocomplete
        //$normal[] ('success' 'username' 'repo')
        echo \print_r($normal['repos']);
    }

    //We don't need to use elseif here, since we use decode_exclusive.
    if ($timeout !== null) {
        \trigger_error(
            Str\format('Received %d seconds timeout', $timeout['timeout']),
        );
    }

    if ($error !== null) {
        \trigger_error(
            Str\format('Endpoint %s is experiencing issues', $endpoint),
        );
    }

    exit(0);
}
