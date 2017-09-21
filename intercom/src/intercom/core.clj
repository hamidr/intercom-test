(ns intercom.core
  (:gen-class)
  (:require [clojure.string :as string]
            [clojure.data.json :as json]
            [intercom.geo :as geo]))

(defn- value-reader
  [key value]
  (if (or (= key :latitude) (= key :longitude))
    (read-string value)
    value))

(defn- convert-line
  [line]
  (json/read-str line
      :key-fn keyword
      :value-fn value-reader))

(defn- convert-lines
  [lines]
  (map convert-line lines))

(def ^:private dublin
  {:latitude 53.339428 :longitude -6.257664})

(defn- tprint
  [customers]
  (doseq [c customers]
    (let [{id :user_id name :name} c
          item (str "ID: " id " Name: " name)]
      (println item))))

(defn- valid?
  [{id   :user_id
    name :name
    lat  :latitude
    long :longitude}]
  (not
   (or (nil? name)
       (nil? id)
       (nil? lat)
       (nil? long))))

(defn- arg-or-default
  [args default]
  (let [addr (first args)]
    (if (nil? addr)
      default
      addr)))

(defn -main
  [& args]
  (->>
   (arg-or-default args "data/customer.json")
   slurp                         ;; read file
   string/split-lines            ;; split by line into separate elements
   convert-lines                 ;; convert to json
   (filter valid?)               ;; filter only valid ones
   (geo/select-within-radius 100 dublin) ;; select those in 100km radius
   (sort-by :user_id)            ;; sort by their :user_id
   tprint                        ;; print the result
   ))
