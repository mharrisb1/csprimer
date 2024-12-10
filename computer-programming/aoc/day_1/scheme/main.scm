(define (file->string path)
  (read-port-to-string (open-input-file path)))

(define (pop lst)
  (reverse (rest (reverse lst))))

(define (string->matrix s)
  (pop (map (lambda (x) (split-many x " ")) (split-many s "\n"))))

(define (transpose matrix)
  (if (null? (first matrix))
    '()
    (cons (map first matrix)
      (transpose (map rest matrix)))))

(displayln (transpose (string->matrix (file->string "../input.txt"))))
