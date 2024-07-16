(require "../src/main.scm")

(define (assert condition message)
  (if (not condition)
    (displayln "Assertion failed:" message)
    (displayln "Test passed")))

;; Valid credit card numbers
(assert (is-valid? "4532015112830366") "Test failed: 4532015112830366 should be valid")
(assert (is-valid? "4916239364668151") "Test failed: 4916239364668151 should be valid")
(assert (is-valid? "6011514433546201") "Test failed: 6011514433546201 should be valid")
(assert (is-valid? "379354508162306") "Test failed: 379354508162306 should be valid (AMEX)")

;; Invalid credit card numbers
(assert (not (is-valid? "4532015112830367")) "Test failed: 4532015112830367 should be invalid")
(assert (not (is-valid? "4916239364668159")) "Test failed: 4916239364668159 should be invalid")
(assert (not (is-valid? "6011514433546202")) "Test failed: 6011514433546202 should be invalid")
(assert (not (is-valid? "379354508162307")) "Test failed: 379354508162307 should be invalid (AMEX)")

(displayln "All tests completed.")
