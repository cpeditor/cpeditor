; Test file, released under MIT License
(ns ^{:doc "Syntax highlighting test file"
      :author "Markus Brenneis"}
    highlighting)

(defn something-else [f xs]
  #_(map #(apply f (% [%])) (cons 1 xs))
  (map #(apply f (% xs)) (cons 1 xs))
  #_[1 '(2)]
  xs)

(def foo [\a \b \n \ucafe \o123 \n
          \newline \tab \space \formfeed \backspace])

(def fizz {#{\a \b}
           #{\n \newline}})

(def fizz' #{{\a \b}
             {\n \newline}})

(defn bar [xs]
  (as-> xs <>
        (cons :a <>)
        (map #(%1 %2) <>) ; TODO improve
        (into <> [:konjure.logic.specs/numShips])))

(def x-2-y
  #_"do \" sth"
  (domonad set-m
    [x #{1.1, (+ -2 +4)}
     y #{1.1, (- -2.0 4.0)}]
    (*' x y)))

(def bases
  (and (= -1 -1N)
       (= 1/4 -2.5e-1)
       (= -1/2 -0.5M)
       (= -0x1Ab -0X1ab)
       (= +2r101010 25R1h)
       (= 39r13 42R10))) ; FIXME this one is not correct

(def ^{:private true}
  (= (last #{#{}}) #{{#{}}}))

(def s "#repl\n")
(def r #"repl")

(defn- stuff!
  [a]
  "This is no \"documentation\"!"
  (= (class #'+) (class #'foo))
  (let [+ -] [(+ a 1) (@#'+ a 1)]))

(defn- throwIllegalArgumentException!
  "Throws an \"IllegalArgumentException\" or
  a js/Error."
  [message]
  #?(:clj  (throw (IllegalArgumentException. message))
     :cljs (throw (js/Error. message))))

(defmacro let-fn "a nonsense macro" [one-binding & body]
  `(+ 1 ~(inc' 1))
  (let [[identifier & fn-body] one-binding]
    `(let [~identifier (fn ~identifier ~@fn-body)]
      ~@body `a#)))

(def state (atom [(= false true) nil]))

(defn something-cool [] (first @state))

(defn- something-different [] (first (into @state [12])))

(defn- instance-getfield [this k] (@(.state this) k))
