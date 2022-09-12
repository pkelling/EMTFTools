# coding: utf-8

import tensorflow as tf
# import tf.keras.layers as layers
import cmsml

# define your model
model = tf.keras.Sequential()
model.add(layers.InputLayer(input_shape=(10,), name="input"))
model.add(layers.Dense(100, activation="tanh"))
model.add(layers.Dense(3, activation="softmax", name="output"))

# train it
...

# convert to binary (.pb extension) protobuf
# with variables converted to constants
cmsml.tensorflow.save_graph("graph.pb.txt", model, variables_to_constants=True)