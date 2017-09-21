(ns intercom.geo-test
  (:require [clojure.test :refer :all]
            [intercom.geo :refer :all]))


;; http://www.movable-type.co.uk/scripts/latlong.html

(deftest test-distance
  (testing "Distance in KM between two points"
    (let [p1 {:latitude 32.9697  :longitude -96.80322}
          p2 {:latitude 29.46786 :longitude -98.53506}]
      (is (= 422.88 (distance p1 p2))))))


(def TehranLatLong
  {:latitude 41.0138 :longitude 28.9497})

(def Istanbul
  {:latitude 35.6719 :longitude 51.424})

(deftest test-check-distance-between-tehran-istanbul
  (testing "Check the distance between Tehran and Istanbul"
    (is (= 2041.99
         (distance TehranLatLong Istanbul)))))

(deftest test-select-within-radius
  (testing "Select the values within the radius from a list"
    (let [center  {:latitude 35.688122 :longitude 51.392759}
          points [{:latitude 35.686121 :longitude 51.410045 :id 1} ;; dt = 1.58
                  {:latitude 35.689895 :longitude 51.410852 :id 2} ;; dt = 1.65
                  {:latitude 35.683957 :longitude 51.393553 :id 3} ;; dt = 0.47
                  {:latitude 35.687756 :longitude 51.396257 :id 4} ;; dt = 0.32
                  {:latitude 35.675759 :longitude 51.429818 :id 5} ;; dt = 3.62
                  {:latitude 35.667492 :longitude 51.415600 :id 6}];; dt = 3.09
          within-1km (select-within-radius 1 center points)
          within-1km-ids (map :id within-1km)
          within-2km (select-within-radius 2 center points)
          within-2km-ids (map :id within-2km)]
      (is (= within-1km-ids [3 4])) ;; just in 1km
      (is (= within-2km-ids [1 2 3 4])) ;; both in 1km and 2km
      )))
