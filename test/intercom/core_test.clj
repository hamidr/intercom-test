(ns intercom.core-test
  (:require [clojure.test :refer :all]
            [intercom.core :refer :all]))

(testing "Conversion from raw to edn-format"
  (let [convert-line #'intercom.core/convert-line
        raw "{\"latitude\": \"52.986375\",
              \"user_id\": 12,
              \"name\": \"Christina McArdle\",
              \"longitude\": \"-6.043701\"}"
        cooked {:latitude 52.986375
                :user_id 12
                :name "Christina McArdle"
                :longitude -6.043701}]
    (is (= (convert-line raw) cooked))))


(testing "Check validity of a line"
  (let [valid? #'intercom.core/valid?
        valid {:latitude 52.986375
               :user_id 12
               :name "Christina McArdle"
               :longitude -6.043701}
        invalid1 {:user_id 12
                  :name "Christina McArdle"
                  :longitude -6.043701}
        invalid2 {:latitude 52.986375
                  :name "Christina McArdle"
                  :longitude -6.043701}
        invalid3 {:latitude 52.986375
                  :user_id 12
                  :longitude -6.043701}
        invalid4 {:latitude 52.986375
                  :user_id 12
                  :name "Christina McArdle"}]
    (is (= true (valid? valid)))
    (is (= false (valid? invalid1)))
    (is (= false (valid? invalid2)))
    (is (= false (valid? invalid3)))
    (is (= false (valid? invalid4)))
    ))


(testing "Get first argument or a default value"
  (let [arg-or-default #'intercom.core/arg-or-default
        t1 (arg-or-default [] "default")
        t2 (arg-or-default ["first"] "default")]

    (is (= t1 "default"))
    (is (= t2 "first"))))
