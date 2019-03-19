<?hh // strict
namespace Lexidor\Json_Hack;
use type Facebook\HackTest\HackTest;
use type Facebook\HackTest\DataProvider;
use function Facebook\FBExpect\expect;
use type InvalidArgumentException;

class JsonDecoderTest extends HackTest {


    public function dataProvider(): vec<(string, mixed)> {
        return vec[
            tuple('[]', vec[]),
            tuple('[1,2,4]', vec[1, 2, 4]),
            tuple('{}', dict[]),
            tuple('{"key": "value"}', dict['key' => 'value']),
        ];
    }

    <<DataProvider('dataProvider')>>
    public function test_json_decoder_uses_fb_arrays_if_no_mapper_is_given(
        string $json,
        mixed $expected,
    ): void {
        expect(json_decoder($json))->toBeSame($expected);
    }

    public function dataProvider2(): vec<(string, mixed)> {
        return vec[
            tuple('[]', vec[]),
            tuple('[1,2,4]', vec[1, 2, 4]),
            tuple('{}', shape()),
            tuple('{"key": "value"}', shape('key' => 'value')),
        ];
    }

    <<DataProvider('dataProvider2')>>
    public function test_json_decoder_uses_vec_and_shape_when_a_mapper_is_given(
        string $json,
        mixed $expected,
    ): void {
        expect(json_decoder($json, $raw ==> $raw))->toBeSame($expected);
    }


    public function test_invalid_json_throws_invalid_argument_exception(
    ): void {
        expect(() ==> json_decoder('{', $raw ==> $raw))->toThrow(
            InvalidArgumentException::class,
            'Syntax error',
        );
    }

    public function test_json_decoder_option_empty_string_error(): void {
        expect(
            () ==> json_decoder(
                '',
                $raw ==> $raw,
                shape('empty_string_error' => true),
            ),
        )->toThrow(InvalidArgumentException::class);
    }

    public function test_json_decoder_option_empty_string_error_is_default(
    ): void {
        expect(() ==> json_decoder('', $raw ==> $raw))->toThrow(
            InvalidArgumentException::class,
            'Empty string given as',
        );
    }

    public function test_json_decoder_option_no_mapping_error_is_ignored(
    ): void {
        expect(
            () ==> json_decoder(
                '[]',
                $raw ==> null,
                shape('no_mapping_error' => true),
            ),
        )->notToThrow();
    }

    public function test_json_decoder_passes_the_decoded_value_to_mapper(
    ): void {
        expect(json_decoder('[3]', $three ==> $three === vec[3]))->toBeTrue();
    }

    public function test_json_decoder_returns_the_mapped_value(): void {
        expect(json_decoder('[1,2,3,4]', $_ ==> 'bananas'))->toBeSame(
            'bananas',
        );
    }

    public function test_json_decoder_applies_decode_depth_limit(): void {
        expect(
            () ==> json_decoder(
                '[[]]',
                $_ ==> true,
                shape('decode_depth_limit' => 1),
            ),
        )->toThrow(
            InvalidArgumentException::class,
            'Maximum stack depth exceeded',
        );
    }

    public function test_scalar_with_mapper_error_is_not_default(): void {
        expect(json_decoder('2', $raw ==> $raw))->toBeNull();
    }

    public function test_scalar_with_mapper_error_throws_invalid_argument_exception(
    ): void {
        expect(
            () ==> json_decoder(
                '2',
                $raw ==> $raw,
                shape('scalar_with_mapper_error' => true),
            ),
        )->toThrow(
            InvalidArgumentException::class,
            'Explicit error for scalar',
        );
    }

    public function test_scalar_with_mapper_error_does_not_throw_when_a_keyed_container_is_decoded(
    ): void {
        expect(
            () ==> json_decoder(
                '[2]',
                $raw ==> $raw,
                shape('scalar_with_mapper_error' => true),
            ),
        )->notToThrow();
    }

}
