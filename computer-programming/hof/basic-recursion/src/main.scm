(provide !)
(provide is-palindrome?)
(provide %)
(provide gcd)
(provide my-filter)
(provide my-reduce)
(provide fib)

(define (! n)
  (if (<= n 1) n (* n (! (- n 1)))))

(define (is-palindrome? s)
  (if (= (string-length s) 1)
    #true
    (and
      (char=? (string-ref s 0) (string-ref s (- (string-length s) 1)))
      (is-palindrome? (substring s 1 (- (string-length s) 1))))))

(define (% a b)
  (- a (* b (floor (/ a b)))))

(define (gcd a b)
  (if (zero? a)
    b
    (if (zero? b)
      a
      (if (> a b)
        (gcd (% a b) b)
        (gcd a (% b a))))))

;; https://stackoverflow.com/a/9459230
(define (my-filter pred lst)
  (cond ((null? lst) (list))
    ((pred (first lst))
      (cons (first lst) (my-filter pred (rest lst))))
    (else (my-filter pred (rest lst)))))

;; https://stackoverflow.com/a/26277420
(define (my-reduce reduction lst acc)
  (if (empty? lst) acc
    (reduction (car lst) (my-reduce reduction (cdr lst) acc))))

(define (fib n)
  (if (<= n 1) n
    (+ (fib (- n 1)) (fib (- n 2)))))
