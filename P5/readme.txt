# Setup

## Environment

* OS: Ubuntu Desktop 20.04
* IDE: PyCharm

# Required Packages

```
tensorflow
tensorflow-gpu (optional)
numpy
matplotlib
opencv
```

## Program

**MNIST Dataset**

1. Train the model: `python mnist_train.py`
2. Evaluate the model: `python mnist_evaluate.py`
3. Visualize the model: `python mnist_visualize.py`

**Greek Dataset**
1. Load and save the dataset: `python greek_dataset.py`
2. Compute the embeddings: `python greek_embeddings.py`
3. Measure the SSD between embeddings: `python greek_evaluate.py`

**Experiment**
1. Run the experiment: `python mnist_experiment.py`