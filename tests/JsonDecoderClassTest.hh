<?hh // strict
namespace Lexidor\Json_Hack;
use type Facebook\HackTest\HackTest;
use type Facebook\HackTest\DataProvider;
use function Facebook\FBExpect\expect;
use type InvalidArgumentException;

class JsonDecoderClassTest extends HackTest {

    public function test_json_decoder_class_passes_the_decoded_value_to_mapper1(
    ): void {
        $decoder =
            new JsonDecoder1($vec12345 ==> $vec12345 === vec[1, 2, 3, 4, 5]);
        expect($decoder->decode('[1,2,3,4,5]'))->toBeTrue();
    }

    <<DataProvider('dataProvider012')>>
    public function test_json_decoder_class_doesnt_catch_the_exceptions_thrown_by_json_decoder(
        dynamic $decoder,
    ): void {
        expect(() ==> $decoder->decode('{'))->toThrow(
            InvalidArgumentException::class,
            'Syntax error',
        );
    }

    public function test_json_decoder1_class_will_throw_on_mapping_error(
    ): void {
        $decoder1 = new JsonDecoder1($_ ==> null);
        expect(() ==> $decoder1->decode('{"a": 1}'))->toThrow(
            InvalidArgumentException::class,
            'decodes have failed for decode',
        );
    }

    public function test_json_decoder2_class_will_throw_on_mapping_error(
    ): void {
        $decoder2 = new JsonDecoder2($_ ==> null, $_ ==> null);
        expect(() ==> $decoder2->decode('{"a": 1}'))->toThrow(
            InvalidArgumentException::class,
            'decodes have failed for decode',
        );
    }


    public function test_json_decoder2_class_will_not_throw_on_decode_exclusive_if_zero_and_no_mapping_error_false(
    ): void {
        $decoder2 = new JsonDecoder2($_ ==> null, $raw ==> null);
        expect(
            $decoder2->decode(
                '[]',
                shape(
                    'decode_exclusive' => true,
                    'no_mapping_error' => false,
                ),
            ),
        )
            ->toBeSame(tuple(null, null));
    }

    public function test_json_decoder2_class_will_not_throw_on_decode_exclusive_if_only_one(
    ): void {
        $decoder2 = new JsonDecoder2($raw ==> $raw, $_ ==> null);
        expect($decoder2->decode('[]', shape('decode_exclusive' => true)))
            ->toBeSame(tuple(vec[], null));
    }

    public function test_json_decoder2_class_will_throw_on_decode_exclusive_if_more_than_one(
    ): void {
        $decoder2 = new JsonDecoder2($raw ==> $raw, $raw ==> $raw);
        expect(
            () ==> $decoder2->decode('[]', shape('decode_exclusive' => true)),
        )->toThrow(
            InvariantException::class,
            'Decode exclusive ought to prevent you from having multiple succesful decodes, but both mappers returned a nonnull value.',
        );
    }

    public function test_json_decoder0_class_preserves_dicts(): void {
        $decoder = new JsonDecoder0();
        expect($decoder->decode('{}'))->toBeSame(dict[]);
    }

    public function test_json_decoder1_class_do_not_preserve_dicts(): void {
        $decoder1 = new JsonDecoder1($raw ==> $raw);
        expect($decoder1->decode('{}'))->toBeSame(shape());
    }


    public function test_json_decoder2_and_up_class_do_not_preserve_dicts(
    ): void {
        $decoder2 = new JsonDecoder2($raw ==> $raw, $raw ==> $raw);
        expect($decoder2->decode('{}'))->toBeSame(tuple(shape(), shape()));
    }
    public function dataProvider012(): vec<(mixed)> {
        $rawFunc = $raw ==> $raw;
        return vec[
            tuple(new JsonDecoder0()),
            tuple(new JsonDecoder1($rawFunc)),
            tuple(new JsonDecoder2($rawFunc, $rawFunc)),
        ];
    }

    public function dataProvider12(): vec<(mixed)> {
        $rawFunc = $raw ==> $raw;
        return vec[
            tuple(new JsonDecoder1($rawFunc)),
            tuple(new JsonDecoder2($rawFunc, $rawFunc)),
        ];
    }
}
