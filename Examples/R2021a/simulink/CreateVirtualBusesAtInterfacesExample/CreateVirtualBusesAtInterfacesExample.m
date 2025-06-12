%% Connect Multiple Output Signals to a Port
% This example shows how to group the output signals of a subsystem or
% model into a virtual bus using Out Bus Element blocks.

% Copyright 2019-2020 The MathWorks, Inc.

%%
% Open the example model, which contains three source blocks.

open_system('VirtualBusOutputCreationModel');

%% 
% Add an Out Bus Element block to the model. An Out Bus Element block is
% similar to a Bus Creator block connected to an Outport block. Connect the
% output of the Chirp Signal block to the Out Bus Element block.
%
% <<../VirtualBusOutputCreationBlock.png>>
%
%%
% The label next to the Out Bus Element block has two parts. The first part
% of the label describes the bus (|OutBus|), and the second part of the
% label describes the bus element (|signal1|). To make identifying elements
% of the bus easier, rename the element by double-clicking |signal1| and
% entering |Chirp|.
%
% <<../VirtualBusOutputCreationLabel.png>>
%
%% 
% *Ctrl*+drag the Out Bus Element block twice to make two copies of the
% block. When copying the block this way, specify whether to create a new
% port or use the existing port. To create one output bus that contains all
% of the signals, choose *Use Existing Port* each time you copy the block,
% then connect the signals.
%
% <<../VirtualBusOutputCreationSelection.png>>
%
%%
% To create hierarchy in the output bus, use dots in the part of the label
% that describes the bus element. Each dot creates a new level of
% hierarchy. Create a nested bus named |NestedBus| by defining the bus
% elements as |NestedBus.Sine| and |NestedBus.Step|, respectively.
%
% <<../VirtualBusOutputCreationHierarchy.png>>
%
% Alternatively, you can connect a bus to an Out Bus Element block. If
% other Out Bus Element blocks correspond to the same port, the bus is
% nested in the output bus. The part of the label that describes the bus
% element provides the name of this nested bus.
%
%%
% Inspect the bus hierarchy in the dialog box for the port by
% double-clicking any of the Out Bus Element blocks.
%
% <<../VirtualBusOutputCreationDialog.png>>
%
%%
% The three Out Bus Element blocks create a virtual bus at the output port
% of the model. To see the virtual bus, reference the model in another
% model or move the Out Bus Element blocks into a subsystem. The line
% styles update when you compile the model. To compile the model, on the
% *Modeling* tab of the Simulink Toolstrip, click *Update Model* or *Run*.
% 
% <<../VirtualBusOutputCreationPort.png>>
%