(require "collections.scm")
(require "math.scm")

(provide is-valid?)

(define (is-valid? digits)
  (define lst (reverse (map char->number (string->list digits 0))))
  (define check-digit (car lst))
  (define sum (apply +
               (map-with-enumeration
                 (lambda (index element) (if (zero? (mod index 2)) (digit-sum (* element 2)) element))
                 (cdr lst))))
  (= check-digit (- 10 (mod sum 10))))
