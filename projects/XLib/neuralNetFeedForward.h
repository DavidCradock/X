#pragma once
#include "PCH.h"

namespace X
{
	// A neuron of a neural network.
	// It represents a single neuron and holds the weights for each input into the neuron
	// as well as the neuron's bias.
	class CNeuron
	{
	public:
		// Constructor.
		// Sets the number of inputs this neuron has, creates each neuron
		// and bias value for the neuron and sets them all to have random
		// values between -1 and 1
		CNeuron(int iNumInputs);

		// Holds the weight values for each of the inputs this neuron has.
		// It also holds the bias of the neuron at the end of the vector.
		std::vector<double> mvecInputWeights;

		// This holds the number of inputs and weights this neuron has.
		// It actually has the number of inputs + 1, as we need an additional weight
		// for the bias of the neuron.
		// We could just call vecInputsWeights.size(), but for speed purposes,
		// we store the value here.
		int miNumInputs;
	};

	// A neuron layer
	// This holds a layer of neurons
	class CNeuronLayer
	{
	public:
		// Constructor.
		// Sets the number of neurons in this layer.
		CNeuronLayer(int iNumNeurons, int iNumInputsPerNeuron);

		// Holds each neuron in this layer
		std::vector<CNeuron> mvecNeurons;

		// This holds the number of neurons in this layer.
		// We could just call mvecNeurons.size(), but for speed purposes,
		// we store the value here.
		int miNumNeurons;
	};

	// This is a "feed forward" neural network.
	// A feed forward neural network is one whereby the inputs go into
	// the network and move along in one direction towards the outputs.
	// The advantages of this is speed.
	// The disadvantages is that it is not as complex.
	// A neural network, regardless of it's type, simulates connections and firings between neurons in a biological brain.
	//
	// Here's a short list of animals and the approximate number of neurons within their minds...
	// Killer whale: 43 100 000 000
	// Dolphin: 18 750 000 000
	// Human: 16 340 000 000
	// Bottlenose dolphin: 12 700 000 000
	// Gorilla (Western): 9 100 000 000
	// Asian elephant: 6 775 000 000
	// Red and green macaw: 2 646 000 000
	// Giraffe: 1 731 000 000
	// Snowy old: 1 270 000 000
	// German shepherd: 885 460 000
	// Lion: 545 240 000
	// Pig: 425 000 000
	// Brown bear: 250 970 000
	// House cat: 249 830 000
	// Mallard duck: 112 255 000
	// Common wood pigeon: 51 325 000
	// Brown rat: 31 000 000
	// Golden hamster: 17 000 000
	// Bat: 6 000 000
	// Ant: 250 000
	// Cockroach: 200 000
	// Honey bee: 170 000
	// Cricket: 50 000
	// Jelly fish: 5 600
	// Common fruit fly: 2 500
	// Starfish: 500
	// Tardigrade: 200
	// Sea sponge: 0
	//
	// We won't be simulating the mind of a killer whale any time soon, but the above list
	// gives us an insight into how complex these creatures' minds are.
	// 
	// In a human mind, each neuron is connected via its dendrites to approximately 10,000 other neurons.
	// This means that it's possible to have 1,000,000,000,000,000 connections!
	// 
	// A brain’s neurons either fire or they don’t. The strength of the emitted signal
	// does not vary, only the frequency. The neuron sums all the incoming signals from
	// the synapses and if the total signal exceeds a threshold	value, the neuron fires and an
	// electrical signal is sent shooting down the axon.
	// Also, each neuron operates at around 100Hz.
	//
	// Obviously, we can't use millions of neurons, but with a number as small as just ten, we are able
	// to get some incredibly interesting behaviour.
	// 
	// The great thing about all these neurons is that they are great for generalizing.
	// and if trained well, can exhibit fantastic creature like behaviour.
	// Neural networks are commonly used for pattern recognition. This is because they
	// are great at mapping an input state (the pattern it’s trying to recognize) to an
	// output state (the pattern it has been trained to recognize)
	// 
	// Let’s take the example of character recognition. Imagine an LED panel made up of a grid of lights 8×8.
	// Each light can be on or off, so the panel can be used to display the numbers 0 through to 9.
	// To solve the problem, a network must be designed which will accept the state of the panel as an input and then
	// output either a one or a zero. A one to indicate that, for example, the character 7 is being displayed and zero
	// if it thinks it is not. The neural net will have 64 inputs, a layer of neurons, all feeding their output into
	// just one neuron in the output layer.
	// Once the neural network has been setup with the inputs, layers and output, it must be trained to recognize the
	// character 7. One way of doing this is to initialize the neural net with random weights and then feed it a series of
	// inputs which represent the different characters shown on the panel. For each character, we check to see what it's
	// output is and adjust the weights accordingly. If the input pattern we feed it is not a 7, then we know the neural network
	// should output a zero. So for every non 7 character, the weights are adjusted slightly so the output tends toward zero.
	// When	it’s presented with a pattern that represents the character 7, the weights are adjusted so the output tends
	// toward the number one.
	//
	// We could increase the number of outputs to ten. Then	it would be possible to train the network to recognize all the
	// digits 0 through 9. But let's go even more nuts! Let’s increase the outputs so the entire alphabet can be recognized.
	// This is how handwriting recognition works. For each character, the network is trained to recognize many different
	// versions of a letter. Eventually the network will not only be able to recognize the letters it has been trained with,
	// but it will also be able to generalize. That is, if a letter is drawn slightly differently from the letters used during
	// training, the network will stand a pretty good chance of recognizing it.
	// 
	// Not only can a network be used for recognition of characters, but any type of image. People's faces, photos of animals
	// or objects. A network can also even be used for horse racing prediction and, interesting to us, game object navigation
	// (flocking, avoidance, moving towards food, away from enemies) and more!
	//
	// This type of network training is called "Supervised training" and the data used to train it is called a "training set".
	// 
	// Now let's get back to programming...
	// We simulate all these neurons and their connections (On a much smaller scale) within this class.
	// Each neuron can have inputs which are either on or off, 0 or 1 and they are scaled by a weight.
	// Each of these weights are summed by their connected neuron and if the sum is over a certain
	// threshold, the neuron fires. Just like a biological neuron, kinda:)
	// The inputs can be positive or negative so can contribute towards or against a neuron firing.
	// A feed forward network has it's inputs at one end and then one or more layers of neurons, with
	// the initial layer taking and summing the inputs, firing or not and then sending that onto the next
	// layer of neurons until the final layer is reached. Each neuron in each layer is connected to each and
	// every neuron in the next layer. The outputs of the final neuron layer are then retrieved by use to interprete however we
	// choose.
	// 
	// The outputs of a network are either 0 or 1, however, we can modify them so that the output is S shaped, with the values
	// being around 0.5. To do this, we use a sigma function, sigma being a Greek word for something which is S shaped.
	// We can then use these modified output values as anything we like, for example, the amount of rotation around an axis, or
	// velocity increase/decrease, or whether to fire a weapon and more.
	//
	// When using the sigmoid method, it requires a value named response. This modifies the curve of the output value and is typically
	// set to 1. Larger values produce a smoother curve where the values tend towards a straight line, whereas smaller values produce
	// a tighter curve where the value centres more towards 0.5.
	//
	// When choosing which inputs to use, the fewer the better because the network won't have to "work as hard" to find a relationship
	// between them all. So for example, if we're trying to make some game entity move towards certain objects, instead of specifying
	// lots of inputs such as entity positions(x and y), vectors towards the objects (also x,y) and the entity's direction vector
	// (another x and y), we could more efficiently give the network just a single float for it's rotation and a single float
	// which represents the angle between the rotation of the entity and the object instead. These fewer inputs still represent
	// all the information needed, but as there are less of them, the network will be able to find the relationship between them easier.
	// And with this, the network can contain fewer neurons and be faster to compute as a result.
	//
	// Inputs should each be given, which are around the same scale, for example if a vector was given to a target and it wasn't normalized,
	// and another input was given, perhaps say the entity's normalized direction vector, the vector with the larger magnitude would
	// have a much greater impact on the neural network than the entity's direction vector and as a result, the network may have
	// a really hard time trying to find a relationship between them.
	// Another great tip is that sometimes, centering the inputs around zero give better results.
	// 
	// What about the number of neurons and number of layers? What should we choose there?
	// It's pretty much down to trial and error and getting a feel for how the network is behaving. Typically, one layer is enough.
	// As for the number of neurons, yup, same again, trial and error. However, too few and the network won't stand a chance at 
	// giving good outputs and too many, well, it'll be OK, but it will be slower to compute.
	//
	// So with all that out of the way, how do we actually use them?
	// We create a network, choosing the number of inputs we'll give it, the number of layers, the number of neurons in each layer
	// and the number of outputs.
	// We then use another class, to train the network. Then once the we've found networks which perform as we wish, we can then
	// save those best performing networks to a file. Then in our game/program, we can create a network, for each entity in our
	// program, load the networks weights and configuration from the file and then update it, passing it it's input we used during
	// training and getting the outputs and applying those the same we did during training, nice.
	// 
	// At the moment, the training class we use is in geneticAlgorithm.h/cpp, go there to see more detail on how we train the networks.
	class CNeuralNetworkFeedForward
	{
	public:
		// Constructor.
		// Sets to default values.
		CNeuralNetworkFeedForward();

		// Constructor.
		// Sets values to the ones given
		CNeuralNetworkFeedForward(int iNumInputs, int iNumOutputs, int iNumLayers, int iNumNeuronsPerLayer);

		// Creates/recreates the network
		void create(int iNumInputs, int iNumOutputs, int iNumLayers, int iNumNeuronsPerLayer);

		// Returns a vector holding all the weights of all the neurons in each of the layers, including the output layer
		std::vector<double> getNeuronWeights(void) const;

		// Returns the number of weights of all the neurons in each of the layers, including the output layer
		int getNumberOfWeights(void) const;

		// Replaces the weights of all the neurons in each of the layers with the ones given.
		void replaceWeights(const std::vector<double>& newWeights);

		// Given a vector of inputs, updates the neural network and returns the output values
		// If invalid number of inputs is given, an exception occurs
		std::vector<double> update(std::vector<double>& vecInputs);

		// Sets the bias which is used during update when computing the output for a neuron (weight * input) * bias
		// This is typically set to -1 and left alone.
		void setWeightBias(double dWeightBias = -1);

		// Sets the response value used by the sigmoid function which sets the shape of the curve produced from the sigmoid function.
		// Higher values, flatten the curve, lower ones tighten it.
		// A default value of 1 is usually used.
		// Do not set it to zero, this'll create a divide by zero error. (Actually, an exception occurs)
		void setSigmoidResponse(double dResponse = 1);

		// Calculate splits points used by the genetic algorithm training class
		// This returns points in the weights vector which are spaced out between each neuron.
		// This makes it so that when we split the weights of the parents, when creating child networks,
		// the splits are between neurons, rather than ANY position which could be between some of the 
		// weights of a single neuron. If this happened, that individual neuron when passed on to the child,
		// could be considered "mutated", as we are not passing on a whole neuron, only a part of it.
		std::vector<int> calculateSplitPoints(void);
	private:

		// The number of inputs this network has
		int _miNumInputs;

		// The number of outputs this network has
		int _miNumOutputs;

		// The number of layers of neurons this network has
		// If this is zero, mvecLayers still contains the output layer.
		int _miNumLayers;

		// The number of neurons per layer this network has
		int _miNumNeuronsPerLayer;

		// The bias which is multiplied to the results of a neuron's weight * input during update.
		// It is typically set to -1
		double _mdBias;

		// Holds the response value used by the sigmoid function
		double _mdSigmoidResponse;

		// Vector holding each layer of neurons (if any)
		// The final layer is the outputs.
		// If the number of layers is set to zero, then there will only be one
		// layer, the output layer.
		std::vector<CNeuronLayer> _mvecLayers;
	};
}