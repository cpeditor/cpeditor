;; This test file tests kates common lisp highlighting
#|
multilinecomment :)
|#

;BEGIN region marker

;END end

(defun bin-search (obj vec)
  (let ((len (length vec)))
    (and (not (zerop len))
         (finder obj vec 0 (- len 1)))))

(defun parse-date (str)
  (let ((toks (tokens str #'constituent 0)))
    (list (parse-integer (first toks))
          (parse-month   (second toks))
          (parse-integer (third toks)))))

(defconstant month-names
  #("jan" "feb" "mar" "apr" "may" "jun"
    "jul" "aug" "sep" "oct" "nov" "dec"))

(defstruct buf
  vec (start -1) (used -1) (new -1) (end -1))

(defparameter *words* (make-hash-table :size 10000))

(defmacro while (test &rest body)
  `(do ()
       ((not ,test))
     ,@body))

(define-modify-macro append1f (val)
  (lambda (lst val) (append lst (list val))))
