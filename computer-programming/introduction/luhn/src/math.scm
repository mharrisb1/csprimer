(provide mod digit-sum)

(define (mod a b)
  (- a (* b (floor (/ a b)))))

(define (digit-sum n)
  (apply + (map char->number (string->list (number->string n 10) 0))))
