# Documentation for JsonDecoder

## Why this could not be expressed in hack
* The return value of JsonDecoder(%d+)::decode is a tuple.
* The length of this tuple is the %d in the classname.
* The types on the tuple f.e. (int, bool) are the return types of the functions given in the constructor.
* Hack does not allow me to express this, so therefore you would theoretically need infinetly many classes.
* Writing an interface for such a series of classes is impossible.

## Interface

### __construct(): JsonDecoder(%d+)
Takes %d parameters of type `(function(mixed): Tyourchoice)` where `Tyourchoice` may be different for each argument.

### decode(string, json_decoder_options): Tuple<?T1, ..., ?Tn>
Attempts to decode arg1 and will try all mappers given in the constructor.
The result is returned to you as a tuple, where the arity is %d+ and the types are in the same order as the returns of the functions given in the `__construct` method.


If the option `decode_exclusive` was set no more than 1 mapper is allowed to succeed.
If the option `no_mapping_error` was set at least 1 mapper is required to succeed.
When both options are given only 1 mapper may succeed.

## Special case
Since `decode_exclusive` guarantees that at max mapper may succeed the real return type of `decode` becomes `(?Tany, null, null, null)` where Tany may be in any slot.


Since `decode_exclusive` and `no_mapping_error` together guarantee that at one mapper will succeed the real return type of `decode` becomes `(Tany, null, null, null)` where Tany may be in any slot.