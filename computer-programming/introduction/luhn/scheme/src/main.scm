(provide is-valid?)

(define (% a b)
  (- a (* b (floor (/ a b)))))

(define (enumerate lst)
  (let loop ((lst lst) (idx 0) (result '()))
    (if (null? lst)
      (reverse result)
      (loop (cdr lst) (+ idx 1) (cons (cons idx (car lst)) result)))))

(define (map-with-enumeration f lst)
  (map (lambda (pair)
        (let ((index (car pair))
              (element (cdr pair)))
          (f index element)))
    (enumerate lst)))

(define (is-valid? digits)
  (define table (vector 0 2 4 6 8 1 3 5 7 9))
  (define lst (reverse (map char->number (string->list digits 0))))
  (define check (first lst))
  (define n (apply +
             (map-with-enumeration
               (lambda (i x) (if (even? i) (vector-ref table x) x))
               (rest lst))))
  (= check (- 10 (% n 10))))
