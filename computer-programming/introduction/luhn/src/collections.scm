(provide enumerate map-with-enumeration)

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
