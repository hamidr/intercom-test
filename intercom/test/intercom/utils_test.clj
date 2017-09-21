(ns intercom.utils-test
  (:require [clojure.test :refer :all]
            [intercom.utils :refer :all]))

(deftest test-flatten
  (testing "Simple flatten action"
    (let [x [1 [2 [3 [4 [5]]]]]
          y [1 2 3 4 5]]
      (is (= (my-flatten x) y))))
  (testing "Flatten-ed nested arrays should be ordered equally"
    (let [x [1 2 3]
          y [[[3]] [2] 1]
          fx (my-flatten x)
          fy (my-flatten y)]
      (is (not (= fx fy)))))
  (testing "Flattening complex nested arrays"
    (let [x [1 2 3]
          y [1 [2] [[3]]]
          z [[1] [2 []] 3]
          fx (my-flatten x)
          fy (my-flatten y)
          fz (my-flatten z)]
      (is (= fx fy fz)))))
