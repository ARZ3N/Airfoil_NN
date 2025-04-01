# -*- coding: utf-8 -*-
"""ESCNN_MODEL.PY
Python script to built, train and run escnn model inference
Author: Aniruddha
Date: Dec-2022

# 1. Import all needed libraries
"""

import tensorflow as tf
import numpy as np
import pandas as pd
from tensorflow.keras import datasets, layers, models
import matplotlib.pyplot as plt
from google.colab import files # This is used to upload data from local files. We can host them on the cloud if neccesary in the future
import math
import os

tf.test.gpu_device_name() # Just to make sure that the GPU is enabled

"""# 2. Load dataset from local files"""

# Important to know that this will essentially discount the first row of data. This means that the first row of the csv needs to be populated with the headers
data = files.upload()

"""# 3. Prepare dataset"""

trainsplit = 0.8 # The percentage of the dataset that will be used for training



filename = 'dataset.csv' # Replace with whatever the final dataset is called on our local machine (we might each name the file(s) a different name)
data = pd.read_csv(filename) # Reading the data with pandas handler
data = data.to_numpy() # Converting pandas dataframe to numpy array for easier handling

np.random.seed(51) # Providing a specified seed for random generator in order to get reproducable results
np.random.shuffle(data) # Shuffling data so that model doesn't "learn bad habbits" (Imagine if there was an unintended pattern for example if the ouput happened to increase by 1)

ReMabool = False # If true, include Re and Ma number in dataset

# This if/else statement controls whether we include the Ma and Re number with each sample
if (ReMabool):
    inputlimit = data.shape[1]-1

else:
    inputlimit = data.shape[1]-3


print("Total Data size:", data.shape[0]) # Verifying that the shape of formatted dataset is correct. Should have 158 columns (157 for input vector and 1 extra row for output)
trainData = data[0:int(trainsplit*data.shape[0]+1),0:inputlimit] # Splitting data array for testing
trainOutput = data[0:trainData.shape[0], data.shape[1]-1]
valData = data[trainData.shape[0]:data.shape[0], 0:inputlimit] # Splitting data array for training
valOutput = data[trainData.shape[0]:data.shape[0], data.shape[1]-1]

assert(trainData.shape[0] + valData.shape[0] == data.shape[0]) # validating that the sum of testing and training data size equals the total data available. Throws an error if false
print("Training Data size is: {}, Testing/Validation size is: {}, Total Data size sum is: {}". format(trainData.shape[0], valData.shape[0], data.shape[0]))

"""# 4. Creating the model"""

model = tf.keras.models.Sequential() # Creating a keras sequential model
model.add(layers.Conv1D(200,kernel_size=5,use_bias=True,activation='relu',input_shape=(trainData.shape[1],1))) # Adding 1D conv layer with 200 chanels and kernal size of 5 with a bias added and with a relu activation
model.add(layers.MaxPooling1D(2)) # Performing max pooling
model.add(layers.Conv1D(200, kernel_size=5,use_bias=True,activation='relu')) # Second 1D conv layer with 200 chanels and kernel size of 5
model.add(layers.MaxPooling1D(2)) # Performing max pooling
model.add(layers.Flatten()) # Flattening data into one vector as input to final dense layer
model.add(layers.Dense(1)) # Final dense layer used for coefficient of lift prediction

model.summary() # Printing model summary to verify model setup was correct

"""# 5. Time to Train"""

model.compile(optimizer='adam', # Compiling model with the "Adam" optimizer
              loss='mse',
              metrics=['mse']) # Also using logarithmic mean squared error since that's what is plotted in the paper

print(trainData.shape)
history = model.fit(trainData, trainOutput, epochs=500, validation_data=(valData,valOutput)) # Train model. The paper uses 1600 epochs

"""# 6. Evaluating Model and creating plots"""

# First to create the plot of training and validation performace
plt.plot(np.log10(history.history['loss']), label='Training')
plt.plot(np.log10(history.history['val_loss']), label='Validation')
plt.xlabel('Epoch')
plt.ylabel('Log MSE Cl')
# plt.yscale("log")
#plt.ylim([-3, -1])
plt.legend(loc="upper right")

test_loss, test_acc, *anythingelse = model.evaluate(valData, valOutput, verbose=2)

# Need to save model

# Next to create the plot of specific airfoil performance vs alpha value
filename = 'SUPERMARINE371i.csv' # Replace with whatever the final dataset is called on our local machine (we might each name the file(s) a different name)
test = pd.read_csv(filename) # Reading the data with pandas handler

test = test.to_numpy() # Converting pandas dataframe to numpy array for easier handling

print(test.shape)

output = model.predict(test[0:,0:160])
# print(test[0:,162])
# print(output)
angles = [-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
plt.scatter(test[0:,162],angles, label='Expected', marker='o')
plt.scatter(output,angles, label='Predicted', marker='^')
plt.plot(test[0:,162],angles, label='Expected')
plt.plot(output,angles, label='Predicted')
plt.xlabel('angle of attack')
plt.ylabel('Coefficient of lift')
# plt.yscale("log")
#plt.ylim([-3, -1])
plt.legend(loc="upper left")

"""# Quantizing and converting to TF lite Model"""

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

airfoil_model = "airfoil_model.tflite"
open(airfoil_model, "wb").write(tflite_model)

interpreter = tf.lite.Interpreter(model_path=airfoil_model)
interpreter.allocate_tensors()

print("== Input details ==")
print("name:", interpreter.get_input_details()[0]['name'])
print("shape:", interpreter.get_input_details()[0]['shape'])
print("type:", interpreter.get_input_details()[0]['dtype'])

print("\n== Output details ==")
print("name:", interpreter.get_output_details()[0]['name'])
print("shape:", interpreter.get_output_details()[0]['shape'])
print("type:", interpreter.get_output_details()[0]['dtype'])

print("\nDUMP INPUT")
print(interpreter.get_input_details()[0])
print("\nDUMP OUTPUT")
print(interpreter.get_output_details()[0])

tf_lite_output = []

input_details = interpreter.get_input_details()
for x in range(13):
  input = np.expand_dims(np.float32(test[x,0:160]), axis=0)
  # print("shape of input:", input.shape)
  input = np.expand_dims(input, axis=2)
  # print("shape of expanded input:", input.shape)

  interpreter.set_tensor(input_details[0]['index'], input)
  interpreter.invoke()

  output_details = interpreter.get_output_details()
  output_data = interpreter.get_tensor(output_details[0]['index'])

  tf_lite_output.append(output_data[0][0])
print(test[0:,162])
print(tf_lite_output)

plt.scatter(test[0:,162],angles, label='Expected', marker='o')
plt.scatter(tf_lite_output,angles, label='Predicted', marker='^')
plt.plot(test[0:,162],angles, label='Expected')
plt.plot(tf_lite_output,angles, label='Predicted')
plt.xlabel('angle of attack')
plt.ylabel('Coefficient of lift TFlite Model')
# plt.yscale("log")
#plt.ylim([-3, -1])
plt.legend(loc="upper left")
print("TFlite Model size:", os.stat('airfoil_model.tflite').st_size)