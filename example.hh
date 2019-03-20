<?hh //strict
namespace Lexidor\Json_Hack\Example;

use type Lexidor\Json_Hack\JsonDecoder3;
use namespace HH\Lib\Str;

<<__EntryPoint>>
function example(): noreturn {
    require_once __DIR__.'/vendor/hh_autoload.hh';

    $endpoint = 'https://api.github.com/...';

    //Let's pretend to curl an api.
    $apiResponse = \json_encode(
        ['success' => true, 'username' => 'Lexidor', 'repo' => 'hack-json'],
    );

    /*
     * We expect one of three results:
     * {"success" => bool, "username" => string, "repo" => string}
     * {"success" => bool, "reason" => string}
     * {"success" => bool, "timeout" => int}
     * And each of them might receive more fields in the future without break BC.
     */

    $decoder = new JsonDecoder3(
        $normal ==> $normal ?as shape(
            "success" => bool,
            "username" => string,
            "repo" => string,
            ...
        ),
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

    list($normal, $error, $timeout) =
        $decoder->decode($apiResponse, shape('decode_exclusive' => true));

    if ($normal !== null) {
        //The result was (as expected) ['success' => 1, 'username' => 'Lexidor', 'repo' => 'hack-json']
        //These fields are now available using autocomplete
        //$normal[] ('success' 'username' 'repo')
        echo 'User loaded...';
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
