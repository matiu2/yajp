project(test_parser)

boost_test(number
    zero
    one
    hundreds
    large_number
    negative_zero
    negative_zero_point_zero
    zero_point_zero
    one_negative_exponent
    positive_exponent_int
    float_to_int_with_exponent
    neg_float_to_int_with_exponent
    neg_neg
    long_double
)

# For each test library copy and paste and change this

boost_test("string"
    empty_string
    simple_string
    carriage
    back_slash
    quote_start
    quote_middle
    quote_end
    quote_three
    lots_of_specials1
    unicode_cents
    unicode_euro
    unicode_kanbun
    unicode_square
    junk_after
)

boost_test(
    person_example
    empty_string
    read_a_person
    only_one_attr
    ignore_junk
)
