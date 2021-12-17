#%%
import numpy as np
import tensorflow as tf
import imageio
import h5py
from tensorflow import keras
from PIL import Image
import tensorflow_io as tfio

def keys(f):
    return [key for key in f.keys()]
#%%
train_set_x = h5py.File('camelyonpatch_level_2_split_train_x.h5','r')
train_set_y = h5py.File('camelyonpatch_level_2_split_train_y.h5','r')
val_set_x = h5py.File('camelyonpatch_level_2_split_valid_x.h5','r')
val_set_y = h5py.File('camelyonpatch_level_2_split_valid_y.h5','r')
test_set_x = h5py.File('camelyonpatch_level_2_split_test_x.h5','r')
test_set_y = h5py.File('camelyonpatch_level_2_split_test_y.h5','r')
x_train=train_set_x['x']
y_train=train_set_y['y']
y_train=np.squeeze(y_train, axis=1)
y_train=np.squeeze(y_train, axis=1)
x_val=val_set_x['x']
y_val=val_set_y['y']
y_val=np.squeeze(y_val, axis=1)
y_val=np.squeeze(y_val, axis=1)
x_test=test_set_x['x']
y_test=test_set_y['y']
y_test=np.squeeze(y_test, axis=1)
y_test=np.squeeze(y_test, axis=1)
#%%

train_dataset = tf.data.Dataset.from_tensor_slices((x_train, y_train))
valid_dataset = tf.data.Dataset.from_tensor_slices((x_val, y_val))
test_dataset = tf.data.Dataset.from_tensor_slices((x_test, y_test))
train_dataset = train_dataset.shuffle(buffer_size=1024)
valid_dataset = valid_dataset.shuffle(buffer_size=1024)
#test_dataset = test_dataset.shuffle(buffer_size=1024)
train_dataset = train_dataset.map(lambda x, y: (tf.image.convert_image_dtype(x, tf.float32), y))
valid_dataset = valid_dataset.map(lambda x, y: (tf.image.convert_image_dtype(x, tf.float32), y))
test_dataset = test_dataset.map(lambda x, y: (tf.image.convert_image_dtype(x, tf.float32), y))
train_dataset = train_dataset.batch(128)
valid_dataset = valid_dataset.batch(256)
test_dataset = test_dataset.batch(128)
# %%
model=tf.keras.models.load_model('model9559')
# %%
model.evaluate(test_dataset, batch_size=128)
results = model.predict(test_dataset, batch_size=128)
# %%
np.savetxt("foo1.csv", results,fmt='%.18e', delimiter=",", header="case, prediction")
print(results.shape)
# %%
optimizer = tf.keras.optimizers.Adam(learning_rate=0.000000001, beta_1=0.9, beta_2=0.999, decay=0.1)
#optimizer = tf.keras.optimizers.Adam(learning_rate=0.00000002, beta_1=0.9, beta_2=0.999)
model.compile(optimizer=optimizer,
                   loss='binary_crossentropy',
                   metrics=['AUC'])
model.fit(train_dataset,epochs=1, batch_size=128)
# %%
optimizer = tf.keras.optimizers.Adam(learning_rate=0.000000001, beta_1=0.9, beta_2=0.999, decay=0.001)
#optimizer = tf.keras.optimizers.Adam(learning_rate=0.00000002, beta_1=0.9, beta_2=0.999)
model.compile(optimizer=optimizer,
                   loss='binary_crossentropy',
                   metrics=['AUC'])
model.fit(valid_dataset,epochs=5, batch_size=256)
# %%
model.evaluate(valid_dataset, batch_size=128)
# %%
model.save('model9559')
# %%
model.summary()

# %%
print(y_test.shape)

# %%
