/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "EulerD3dt3Scheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::EulerD3dt3Scheme::~EulerD3dt3Scheme()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

tmp<volScalarField>
EulerD3dt3Scheme::d3dt3
(
    const volScalarField& vf
)
{
      
    IOobject d3dt3IOobject
    (
        "d3dt3("+vf.name()+')',
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
    );

    scalar dt = mesh().time().deltaTValue();
    scalar dt0 = mesh().time().deltaT0Value();

    dimensionedScalar rDeltaT3 = 6./
        ((dt+dt0)*dt*dt0*dt00_*(dt0+dt00_)*(dt+dt0+dt00_));
    
    scalar coefft   = dt0*dt00_*(dt0+dt00_);
    scalar coefft0  = dt00_ * (dt*dt+2*dt*dt0+dt*dt00_+dt0*dt0+dt0*dt00_);
    scalar coefft00  = dt * (dt*dt0+dt*dt00_+dt0*dt0+2*dt0*dt00_+dt00_*dt00_);
    scalar coefft000 = dt*dt0*(dt+dt0);

    return tmp<volScalarField>
        (
            new volScalarField
            (
                d3dt3IOobject,
                rDeltaT3*
                (
                    coefft*vf
                  - coefft0*vf.oldTime()
                  + coefft00*vf.oldTime().oldTime()
                  - coefft000*vf.oldTime().oldTime().oldTime()
                )
            )
        );
   
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
