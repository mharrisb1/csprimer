# Varint Module Notes

## Links

1. [Base 128 varints](https://protobuf.dev/programming-guides/encoding/#varints)

## Explainers

<details>

<summary>Signed vs. Unsigned Integers</summary>

At a simplfied level, we can say that:

- unsigned integers contains the set of [natural numbers](https://en.wikipedia.org/wiki/Natural_number) ($\mathbb{N}$)
- signed integers contains the set of [integers](https://en.wikipedia.org/wiki/Integer) ($\mathbb{Z}$)

Because computers need to work with discrete chunks of data we need to usually[^1] deal with sized integers
so for example a 4-bit unsigned integer (not usually something that is supported) could represent any natural
number from [0, 15]. For example, you would have something like `0101` which would represent 5.

When dealing with *signed* integers, the most significant bit will either be a 0 for positive integers or 1 for
negative integers. So, again if we used a size of 4-bits we could represent any number from [-7, 7]. For example,
if you had something like `1001`, that would be equal to -1.

In real life, the above scheme for signed integers doesn't really work well since you can't use the same
adder circuitry for both unsigned and signed intgers, generally involving a simple gradeschool algorithm,
with carries.

The common scheme used is called [two's complement](https://en.wikipedia.org/wiki/Two%27s_complement) where the
most significant bit in a 4-bit unsigned integer actually corresponds to -8. This is less intuitive to humans
but is much more friendly for the machine. 

[^1]: Some programming languages like Python support variable size integers.

</details>

<details>

<summary>What does it mean for a value to be a certain number of bit?</summary>

- For a lot of higher-level languages like Python and JS, you don't have to think about the size of data types like integers
- In lower-level languages (C/C++, Rust, C#, Java, Go) you do need to think about this becuase of things like overflows/underflows, efficiency, etc.

Example:

Let's say we have a 4-bit integer (unsigned for simplicity) of `1111` (15 in decimal) and we added 1 to it
then we would have an overflow and the result would actually be `0000` (0 in decimal.

So, to make sure that we don't run into issue like this, we need to pick a size for our data that
gives us enough room necessary for our operations.

A real life example of a catastrophic error resulting from an overflow is the [Ariane flight V88](https://en.wikipedia.org/wiki/Ariane_flight_V88).

A more pervasive issue seen in the software engineering is Postgres uses a 32-bit index for the transactions table, so when companies
reach a point where they hit above roughly 4 billion transactions (technically $2^32$) the index overflows back to 0. If the team
hasn't cleaned up the old transactions then this becomes a problem.

</details>
