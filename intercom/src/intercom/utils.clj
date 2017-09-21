(ns intercom.utils
  (:refer-clojure :rename {flatten core-flatten}))

(defn my-flatten
  "Receives a collection and flattens it; e.g.: [x [y[z]] to [x y z].
  Recursively mapping the my-flatten function over every collection and then concatenating
  them from down to up."
  [arg]
  (if (coll? arg)
    (mapcat my-flatten arg)
    [arg]))
