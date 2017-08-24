(ns intercom.geo)

;; https://github.com/ThomasMeier/haversine
;; With a few changes for rounding numbers!

(def earth-radius
  "In kilometers"
  6372.8)

(defn- sin2
  "Sine squared"
  [theta]
  (* (Math/sin theta) (Math/sin theta)))

(defn- alpha
  "Trigonometric calculations for use in haversine."
  [lat1 lat2 delta-lat delta-long]
  (+ (sin2 (/ delta-lat 2))
     (* (sin2 (/ delta-long 2)) (Math/cos lat1) (Math/cos lat2))))

(defn- round2
  "Round a double to the given precision (number of significant digits)"
  [precision d]
  (let [factor (Math/pow 10 precision)]
    (/ (Math/round (* d factor)) factor)))

(defn distance
  "Recieves two points in on earth using latitude and longitude calculates the distance in KM"
  [{lat1 :latitude long1 :longitude}
   {lat2 :latitude long2 :longitude}]
  (let [delta-lat (Math/toRadians (- lat2 lat1))
        delta-long (Math/toRadians (- long2 long1))
        lat1 (Math/toRadians lat1)
        lat2 (Math/toRadians lat2)]
    (round2 2
            (* earth-radius 2
               (Math/asin (Math/sqrt (alpha lat1 lat2 delta-lat delta-long)))))))

(defn- within-distance?
  [n center point]
  (let [d (distance center point)]
    (< d n)))

(defn select-within-radius
  [n center points]
  (filter #(within-distance? n center %) points))
